#pragma once

#include <functional>
#include <memory>

namespace wnet {
	struct Log;
	typedef std::pair<std::shared_ptr<Log>, std::function<void(std::shared_ptr<Log>)>> LogStreamParam;

	class LogStream {
	public:
		LogStream(const LogStreamParam& param);
		~LogStream();
		LogStream& GetStream() { return *this; }

		LogStream& operator<<(bool value);
		LogStream& operator<<(int8_t v);
		LogStream& operator<<(uint8_t v);
		LogStream& operator<<(int16_t v);
		LogStream& operator<<(uint16_t v);
		LogStream& operator<<(int32_t v);
		LogStream& operator<<(uint32_t v);
		LogStream& operator<<(int64_t v);
		LogStream& operator<<(uint64_t v);
		LogStream& operator<<(float v);
		LogStream& operator<<(double v);
		LogStream& operator<<(const std::string& v);
		LogStream& operator<<(const char* v);
		LogStream& operator<<(char v);

	private:
		LogStream(const LogStream&) = delete;
		LogStream& operator=(const LogStream&) = delete;
	private:
		std::shared_ptr<Log> _log;
		std::function<void(std::shared_ptr<Log>)> _call_back;
	};
}