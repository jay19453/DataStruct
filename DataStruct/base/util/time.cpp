#include "time.h"
#include <thread>
#include <ctime>
namespace wnet {
	std::string GetFormatTime(FormatTimeUnit unit/* = FTU_MILLISECOND*/)
	{
		char buf[format_time_buffer_size] = { 0 };
		uint32_t len = format_time_buffer_size;

		GetFormatTime(buf, len, unit);
		return std::move(std::string(buf, len));
	}
	void GetFormatTime(char* buf, uint32_t& len, FormatTimeUnit uint /*= FTU_MILLISECOND*/)
	{
		std::chrono::system_clock::time_point now_time = std::chrono::system_clock::now();
		std::time_t now_time_t = std::chrono::system_clock::to_time_t(now_time);
		
		int32_t millisecond = 0;
		if (uint == FTU_MILLISECOND)
		{
			auto msec = std::chrono::duration_cast<std::chrono::milliseconds>(now_time.time_since_epoch()).count();// ÄÉÃë×ªÎ¢Ãë
			auto sec = std::chrono::duration_cast<std::chrono::seconds>(now_time.time_since_epoch()).count();
			millisecond = (int32_t)(msec - (sec * 1000));
		}

		tm time;
		localtime_s(&time,&now_time_t);
		
		switch (uint)
		{
		case wnet::FTU_YEAR:
			len = snprintf(buf,len,"%04d", 1900 + time.tm_year);
			break;
		case wnet::FTU_MONTH:
			len = snprintf(buf, len, "%04d-%02d", 1900 + time.tm_year,1 + time.tm_mon);
			break;
		case wnet::FTU_DAY:
			len = snprintf(buf, len, "%04d-%02d-%02d", 1900 + time.tm_year, 1 + time.tm_mon,time.tm_mday);
			break;
		case wnet::FTU_HOUR:
			len = snprintf(buf, len, "%04d-%02d-%02d %02d", 1900 + time.tm_year, 1 + time.tm_mon, time.tm_mday,time.tm_hour);
			break;
		case wnet::FTU_MINUTE:
			len = snprintf(buf, len, "%04d-%02d-%02d %02d:%02d", 1900 + time.tm_year, 1 + time.tm_mon, time.tm_mday, time.tm_hour,time.tm_min);
			break;
		case wnet::FTU_SECOND:
			len = snprintf(buf, len, "%04d-%02d-%02d %02d:%02d:%02d", 1900 + time.tm_year, 1 + time.tm_mon, time.tm_mday, time.tm_hour, time.tm_min,time.tm_sec);
			break;
		case wnet::FTU_MILLISECOND:
			len = snprintf(buf, len, "%04d-%02d-%02d %02d:%02d:%02d:%03d", 1900 + time.tm_year, 1 + time.tm_mon, time.tm_mday, time.tm_hour, time.tm_min, time.tm_sec,millisecond);
			break;
		default:
			len = snprintf(buf, len, "%04d-%02d-%02d %02d:%02d:%02d:%03d", 1900 + time.tm_year, 1 + time.tm_mon, time.tm_mday, time.tm_hour, time.tm_min, time.tm_sec, millisecond);
			break;
		}
	}
	uint64_t UTCTimeSec()
	{
		std::chrono::system_clock::time_point now_time = std::chrono::system_clock::now();
		std::time_t now_time_t = std::chrono::system_clock::to_time_t(now_time);
		return std::chrono::duration_cast<std::chrono::seconds>(now_time.time_since_epoch()).count();
	}
	uint64_t UTCTimeMsec()
	{
		std::chrono::system_clock::time_point now_time = std::chrono::system_clock::now();
		std::time_t now_time_t = std::chrono::system_clock::to_time_t(now_time);
		return std::chrono::duration_cast<std::chrono::milliseconds>(now_time.time_since_epoch()).count();
	}
	void Sleep(uint32_t interval)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(interval));
	}
}