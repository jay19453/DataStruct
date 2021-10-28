#pragma once
#include <mutex>
#include "LogBase.h"

namespace wnet {
	class StdoutLogger :
		public BaseLogger {
	public:
		StdoutLogger();
		~StdoutLogger();

		void Debug(std::shared_ptr<Log>& log);
		void Info(std::shared_ptr<Log>& log);
		void Warn(std::shared_ptr<Log>& log);
		void Error(std::shared_ptr<Log>& log);
		void Fatal(std::shared_ptr<Log>& log);
	private:
		std::mutex _mutex;
	};
}