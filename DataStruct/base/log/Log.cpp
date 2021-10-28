#include <stdarg.h>
#include "Log.h"
#include "LogManager.h"

namespace wnet {
	SingletonLogger::SingletonLogger() {
		_logger_m = std::make_shared<LoggerManager>(__log_cache_size,__log_block_size);
	}

	SingletonLogger::~SingletonLogger()
	{

	}

	void SingletonLogger::Debug(const char* file, uint32_t line, const char* log...)
	{
		if (_logger_m)
		{
			va_list list;
			va_start(list, log);
			_logger_m->Debug(file, line, log, list);
			va_end(list);
		}
	}

	void SingletonLogger::Info(const char* file, uint32_t line, const char* log...)
	{
		if (_logger_m)
		{
			va_list list;
			va_start(list, log);
			_logger_m->Info(file, line, log, list);
			va_end(list);
		}
	}

	void SingletonLogger::Warn(const char* file, uint32_t line, const char* log...)
	{
		if (_logger_m)
		{
			va_list list;
			va_start(list, log);
			_logger_m->Warn(file, line, log, list);
			va_end(list);
		}
	}

	void SingletonLogger::Error(const char* file, uint32_t line, const char* log...)
	{
		if (_logger_m)
		{
			va_list list;
			va_start(list, log);
			_logger_m->Error(file, line, log, list);
			va_end(list);
		}
	}

	void SingletonLogger::Fatal(const char* file, uint32_t line, const char* log...)
	{
		if (_logger_m)
		{
			va_list list;
			va_start(list, log);
			_logger_m->Fatal(file, line, log, list);
			va_end(list);
		}
	}

	void SingletonLogger::SetLevel(LogLevel level) {
		if (_logger_m)
		{
			_logger_m->SetLevel(level);
		}
	}

	void SingletonLogger::SetLogger(std::shared_ptr<BaseLogger> log)
	{
		if (_logger_m)
		{
			_logger_m->SetLogger(log);
		}
	}

	LogStreamParam SingletonLogger::GetStreamParam(LogLevel level, const char* file, uint32_t line)
	{
		if (_logger_m)
		{
			return _logger_m->GetStreamParam(level, file, line);
		}
		else return std::make_pair(nullptr,nullptr);
	}
}