#pragma once
#include <fstream>
#include "LogBase.h"
#include "../thread/ThreadWithQueue.h"

namespace wnet {
	static const uint8_t __file_logger_time_buf_size = sizeof("xxxx-xx-xx xx");

	enum FileLoggerSplitUnit {
		FLSU_DAY  = 1,
		FLSU_HOUR = 2,
	};

	class FileLogger :
		public BaseLogger,
		public ThreadWithQueue<std::shared_ptr<Log>> {
	public:
		FileLogger(const std::string& file,
			FileLoggerSplitUnit unit = FLSU_DAY,
			uint16_t max_store_days = 3,
			uint16_t time_offset = 5);
		~FileLogger();

		void Run();
		void StopThread();

		void Debug(std::shared_ptr<Log>& log);
		void Info(std::shared_ptr<Log>& log);
		void Warn(std::shared_ptr<Log>& log);
		void Error(std::shared_ptr<Log>& log);
		void Fatal(std::shared_ptr<Log>& log);

		void SetFileName(const std::string name) { _file_name = name; }
		std::string GetFileName() { return _file_name; }
	private:
		void CheckTime(char* log);
		void CheckExpireFiles();
	private:
		std::string _file_name;
		std::fstream _stream;

		uint16_t _time_offset;
		uint8_t  _time_buf_len;
		FileLoggerSplitUnit _split_unit;
		char _time[__file_logger_time_buf_size];
		std::queue<std::string> _history_file_names;

		uint16_t _max_file_num;

	};
}