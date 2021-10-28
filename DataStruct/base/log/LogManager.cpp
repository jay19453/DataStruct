#include "LogManager.h"
#include "../alloter/NormalAlloter.h"
#include "../util/time.h"
#include "LogBase.h"

namespace wnet {

	enum LogLevelMask {
		LLM_FATAL		= 0x01,
		LLM_ERROR		= 0x02,
		LLM_WARN		= 0x04,
		LLM_INFO		= 0x08,
		LLM_DEBUG		= 0x10,
	};

	//[level|time|file:line] [info|2021-10-25 17:46:40:300|logstream.cpp:30]
	static uint32_t FormatLog(const char* file,uint32_t line, const char* level,char* buf,uint32_t len) {
		uint32_t curlen = snprintf(buf, len, "[%s|", level);

		uint32_t size = format_time_buffer_size;
		GetFormatTime(buf + curlen,size);
		curlen += size;

		curlen += snprintf(buf + curlen, len - curlen, "|%s:%d]", file, line);
		return curlen;
	}

	static uint32_t FormatLog(const char* file, uint32_t line, const char* level, const char* content, va_list list, char* buf, uint32_t len)
	{
		uint32_t curlen = FormatLog(file, line, level, buf, len);

		curlen += vsnprintf(buf+curlen,len - curlen,content,list);
		return curlen;
	}

	LoggerManager::LoggerManager(uint16_t cache_size, uint16_t block_size) :
		_cache_size(cache_size),
		_block_size(block_size) {
		_allocter = MakeNormalAlloterPrt();
	}

	LoggerManager::~LoggerManager()
	{
		SetLevel(LL_NULL);
	}

	void LoggerManager::SetLevel(LogLevel level)
	{
		_level = level;
		if (_level > LL_NULL && _cache_queue.empty())
		{
			for (int i = 0; i < _cache_size;i++)
			{
				_cache_queue.push(NewLog());
			}
		}
		else if (_level == LL_NULL)
		{
			int32_t size = _cache_queue.size();
			for (int i = 0; i < size;i++)
			{
				Log* log = nullptr;
				void* del = nullptr;
				if (_cache_queue.pop(log))
				{
					del = (void*)log;
					_allocter->Free(del);
				}
			}
		}
	}

	void LoggerManager::Debug(const char* file, uint32_t line, const char* content, va_list list) {
		if (!(_level & LLM_DEBUG))
		{
			return;
		}

		std::shared_ptr<Log> log = GetLog();
		log->_len = FormatLog(file, line, "DEG", content, list, log->_log, log->_len);
		if (_logger) _logger->Debug(log);
	}

	void LoggerManager::Info(const char* file, uint32_t line, const char* content, va_list list)
	{
		if (!(_level & LLM_INFO))
		{
			return;
		}

		std::shared_ptr<Log> log = GetLog();
		log->_len = FormatLog(file, line, "INF", content, list, log->_log, log->_len);
		if (_logger) _logger->Info(log);
	}

	void LoggerManager::Error(const char* file, uint32_t line, const char* content, va_list list) {
		if (!(_level & LLM_ERROR))
		{
			return;
		}

		std::shared_ptr<Log> log = GetLog();
		log->_len = FormatLog(file, line, "ERR", content, list, log->_log, log->_len);
		if (_logger) _logger->Error(log);
	}

	void LoggerManager::Warn(const char* file, uint32_t line, const char* content, va_list list) {
		if (!(_level & LLM_WARN))
		{
			return;
		}

		std::shared_ptr<Log> log = GetLog();
		log->_len = FormatLog(file, line, "WAR", content, list, log->_log, log->_len);
		if (_logger) _logger->Warn(log);
	}

	void LoggerManager::Fatal(const char* file, uint32_t line, const char* content, va_list list) {
		if (!(_level & LLM_FATAL))
		{
			return;
		}

		std::shared_ptr<Log> log = GetLog();
		log->_len = FormatLog(file, line, "FAT", content, list, log->_log, log->_len);
		if (_logger) _logger->Fatal(log);
	}

	LogStreamParam LoggerManager::GetStreamParam(LogLevel level,
		const char* file, uint32_t line)
	{
		if (level > _level || level == LL_NULL)
		{
			return std::make_pair(nullptr, nullptr);
		}

		std::shared_ptr<Log> log = GetLog();
		std::function<void(std::shared_ptr<Log>)> cb;
		switch (level)
		{
			/*case wnet::LL_NULL:
				break;*/
		case wnet::LL_FATAL:
			log->_len = FormatLog(file, line, "FAT", log->_log, log->_len);
			cb = [this](std::shared_ptr<Log>log) {_logger->Fatal(log); };
			break;
		case wnet::LL_ERROR:
			log->_len = FormatLog(file, line, "ERR", log->_log, log->_len);
			cb = [this](std::shared_ptr<Log>log) {_logger->Error(log); };
			break;
		case wnet::LL_WARN:
			log->_len = FormatLog(file, line, "WAR", log->_log, log->_len);
			cb = [this](std::shared_ptr<Log>log) {_logger->Warn(log); };
			break;
		case wnet::LL_INFO:
			log->_len = FormatLog(file, line, "INF", log->_log, log->_len);
			cb = [this](std::shared_ptr<Log>log) {_logger->Info(log); };
			break;
		case wnet::LL_DEBUG:
			log->_len = FormatLog(file, line, "DEG", log->_log, log->_len);
			cb = [this](std::shared_ptr<Log>log) {_logger->Debug(log); };
			break;
		default:
			return std::make_pair(nullptr, nullptr);
		}
		return make_pair(log, cb);
	}

	Log* LoggerManager::NewLog()
	{
		Log* item = (Log*)_allocter->AllocAlign(_block_size + sizeof(Log));
		item->_len = _block_size;
		item->_log = (char*)item + sizeof(Log);
		return item;
	}

	void LoggerManager::FreeLog(Log* log) {
		if (_cache_queue.size() > _cache_size)
		{
			void* del = (void*)log;
			_allocter->Free(del);
		}
		else
		{
			log->_len = _block_size;
			_cache_queue.push(log);
		}
	}

	std::shared_ptr<Log> LoggerManager::GetLog()
	{
		Log* log = nullptr;
		if (_cache_queue.pop(log))
		{
		}
		else {
			log = NewLog();
		}
		return std::make_shared<Log>(log, [this](Log* &l) {FreeLog(l); });
	}


}
