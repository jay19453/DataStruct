#include "FileLogger.h"

namespace wnet {
	FileLogger::FileLogger(const std::string& file, 
		FileLoggerSplitUnit unit /* = FLSU_DAY */, 
		uint16_t max_store_days /* = 3 */, 
		uint16_t time_offset /* = 5 */):
		_file_name(file),
		_split_unit(unit),
		_time_offset(time_offset)
	{
		if (unit == FLSU_HOUR)
		{
			_time_buf_len = 13; //xxxx-xx-xx xx
			_max_file_num = max_store_days * 24;
		}
		else {
			_time_buf_len = 10;
			_max_file_num = max_store_days;
		}

		memset(_time, 0, __file_logger_time_buf_size);

		StartThread();
	}

	FileLogger::~FileLogger()
	{
		StopThread();
		JoinThread();
		_stream.close();
	}

	void FileLogger::Run()
	{
		while (!_stop)
		{
			std::shared_ptr<Log> log = Pop();
			if (log)
			{
				CheckTime(log->_log);
				if (_stream.is_open())
				{
					_stream.write(log->_log, log->_len);
					_stream.put('\n');
					_stream.flush();
				}
			}else break;
		}
	}

	void FileLogger::StopThread()
	{
		_stop = true;
		this->Push(nullptr);
	}

	void FileLogger::Debug(std::shared_ptr<Log>& log)
	{
		this->Push(log);
		__super::Debug(log);
	}

	void FileLogger::Info(std::shared_ptr<Log>& log)
	{
		this->Push(log);
		__super::Info(log);
	}

	void FileLogger::Warn(std::shared_ptr<Log>& log)
	{
		this->Push(log);
		__super::Warn(log);
	}

	void FileLogger::Error(std::shared_ptr<Log>& log)
	{
		this->Push(log);
		__super::Debug(log);
	}

	void FileLogger::Fatal(std::shared_ptr<Log>& log)
	{
		this->Push(log);
		BaseLogger::Debug(log);
	}

	//日志名字是从Log中_log提取 因为里面有日期
	//需要先检测_time和log中时间是否相等 相等表示已经存在此文件log 
	void FileLogger::CheckTime(char* log)
	{
		if (strncmp(_time,log + _time_offset,_time_buf_len) == 0)
		{
			return;
		}
		
		if (_stream.is_open())
		{
			_stream.close();
		}

		memcpy(_time, log + _time_offset, _time_buf_len);
		std::string filename(_file_name);
		filename.append("_");
		filename.append(_time, _time_buf_len);
		filename.append(".log");

		_history_file_names.push(filename);
		CheckExpireFiles();
		_stream.open(filename, std::ios::app);
	}

	void FileLogger::CheckExpireFiles()
	{
		if (_history_file_names.size() > _max_file_num)
		{
			std::string  del_file = _history_file_names.front();
			_history_file_names.pop();
			std::remove(del_file.c_str());
		}
	}
}