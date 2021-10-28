#include <iostream>
#include "StdoutLogger.h"

namespace wnet {
	StdoutLogger::StdoutLogger()
	{}
	StdoutLogger::~StdoutLogger()
	{}

	void StdoutLogger::Debug(std::shared_ptr<Log>& log)
	{
		if (log)
		{
			{
				std::lock_guard<std::mutex>lock(_mutex);
				std::cout << log->_log << std::endl;
			}	
			__super::Debug(log);
		}
	}
	void StdoutLogger::Info(std::shared_ptr<Log>& log)
	{
		if (log)
		{
			{
				std::lock_guard<std::mutex>lock(_mutex);
				std::cout << log->_log << std::endl;
			}
			__super::Info(log);
		}
	}
	void StdoutLogger::Warn(std::shared_ptr<Log>& log)
	{
		if (log)
		{
			{
				std::lock_guard<std::mutex>lock(_mutex);
				std::cout << log->_log << std::endl;
			}
			__super::Warn(log);
		}
	}
	void StdoutLogger::Error(std::shared_ptr<Log>& log)
	{
		if (log)
		{
			{
				std::lock_guard<std::mutex>lock(_mutex);
				std::cout << log->_log << std::endl;
			}
			__super::Error(log);
		}
	}
	void StdoutLogger::Fatal(std::shared_ptr<Log>& log) {
		if (log)
		{
			{
				std::lock_guard<std::mutex>lock(_mutex);
				std::cout << log->_log << std::endl;
			}
			__super::Fatal(log);
		}
	}
}