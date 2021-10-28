#pragma once
#include <memory>
#include <cstdint>

namespace wnet {
	struct Log {
		char* _log;
		uint32_t _len;
	};

	//log base
	class BaseLogger {
	public:
		BaseLogger() {}
		virtual ~BaseLogger() {}

		void SetLogger(std::shared_ptr<BaseLogger> logger) {
			_logger = logger;
		}
		std::shared_ptr<BaseLogger> GetLogger() { return _logger; }

		virtual void Debug(std::shared_ptr<Log>& log) { if (_logger) _logger->Debug(log); }
		virtual void Info(std::shared_ptr<Log>& log) { if (_logger) _logger->Info(log); }
		virtual void Warn(std::shared_ptr<Log>& log) { if (_logger) _logger->Warn(log); }
		virtual void Error(std::shared_ptr<Log>& log) { if (_logger) _logger->Error(log); }
		virtual void Fatal(std::shared_ptr<Log>& log) { if (_logger) _logger->Fatal(log); }

	private:
		std::shared_ptr<BaseLogger> _logger; //指向子类类型 能够实现多种日志形式 比如日志log和标准输出log
	};
}
