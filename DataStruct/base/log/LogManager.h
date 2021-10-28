#pragma once
#include <stdint.h>
#include <memory>
#include "../struct/ThreadSafeQueue.h"
#include "Log.h"
#include "LogStream.h"

namespace wnet {
	struct Log;
	class  BaseLogger;
	class  IAlloter;
	//log的管理器 管理Log队列和日志器 
	class LoggerManager {
	public:
		LoggerManager(uint16_t cache_size,uint16_t block_size);
		~LoggerManager();
		
		void SetLogger(std::shared_ptr<BaseLogger> logger) { _logger = logger; }
		void SetLevel(LogLevel level);

		void Debug(const char* file, uint32_t line,
			const char* content, va_list list);

		void Info(const char* file, uint32_t line,
			const char* content, va_list list);

		void Warn(const char* file, uint32_t line,
			const char* content, va_list list);

		void Error(const char* file, uint32_t line,
			const char* content, va_list list);

		void Fatal(const char* file, uint32_t line,
			const char* content, va_list list);

		LogStreamParam GetStreamParam(LogLevel level,
			const char* file, uint32_t line);
	private:
		std::shared_ptr<Log> GetLog();
		void FreeLog(Log* log);
		Log* NewLog();

	protected:
		uint16_t _level;
		uint16_t _cache_size;
		uint16_t _block_size;

		std::shared_ptr<IAlloter> _allocter;
		ThreadSafeQueue<Log*> _cache_queue;
		std::shared_ptr<BaseLogger> _logger;
	};
}