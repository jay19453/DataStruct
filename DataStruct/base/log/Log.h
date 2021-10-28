#pragma once
#include <stdint.h>
#include "../util/Singleton.h"
#include "LogStream.h"

namespace wnet {
	enum LogLevel {
		LL_NULL			= 0x00,
		LL_FATAL		= 0x01,
		LL_ERROR		= 0x02 | LL_FATAL,
		LL_WARN			= 0x04 | LL_ERROR,
		LL_INFO			= 0x08 | LL_WARN,
		LL_DEBUG		= 0x10 | LL_INFO,
	};

	#define LOG_SET(log)			SingletonLogger::get_instance().SetLogger(log)
	#define LOG_SET_LEVEL(level)	SingletonLogger::get_instance().SetLevel(level)

	#define LOG_DEBUG(log,...)		SingletonLogger::get_instance().Debug(__FILE__,__LINE__,log,##__VA__ARGS__)
	#define LOG_INFO(log,...)		SingletonLogger::get_instance().Info(__FILE__,__LINE__,log,##__VA__ARGS__)
	#define LOG_WARN(log,...)		SingletonLogger::get_instance().Warn(__FILE__,__LINE__,log,##__VA__ARGS__)
	#define LOG_ERROR(log,...)		SingletonLogger::get_instance().Error(__FILE__,__LINE__,log,##__VA__ARGS__)
	#define LOG_FATAL(log,...)		SingletonLogger::get_instance().Fatal(__FILE__,__LINE__,log,##__VA__ARGS__)

	#define LOG_DEBUG_S				LogStream(wnet::SingletonLogger::get_instance().GetStreamParam(wnet::LL_DEBUG,__FILE__,__LINE__))
	#define LOG_INFO_S				LogStream(wnet::SingletonLogger::get_instance().GetStreamParam(wnet::LL_INFO,__FILE__,__LINE__))
	#define LOG_WARN_S				LogStream(wnet::SingletonLogger::get_instance().GetStreamParam(wnet::LL_WARN,__FILE__,__LINE__))
	#define LOG_ERROR_S				LogStream(wnet::SingletonLogger::get_instance().GetStreamParam(wnet::LL_ERROR,__FILE__,__LINE__))
	#define LOG_FATAL_S				LogStream(wnet::SingletonLogger::get_instance().GetStreamParam(wnet::LL_FATAL,__FILE__,__LINE__))

	static const uint16_t __log_cache_size = 20;
	static const uint16_t __log_block_size = 1024;

	class BaseLogger;
	class LoggerManager;
	//interface class for user
	class SingletonLogger :
		public Singleton<SingletonLogger>{
	public:
		SingletonLogger();
		~SingletonLogger();

		void SetLogger(std::shared_ptr<BaseLogger> log);

		void SetLevel(LogLevel level);

		void Debug(const char* file,uint32_t line,const char* log...);
		void Info(const char* file, uint32_t line, const char* log...);
		void Warn(const char* file, uint32_t line, const char* log...);
		void Error(const char* file, uint32_t line, const char* log...);
		void Fatal(const char* file, uint32_t line, const char* log...);

		LogStreamParam GetStreamParam(LogLevel level,const char* file,uint32_t line);

	private:
		std::shared_ptr<LoggerManager> _logger_m;
	};
}