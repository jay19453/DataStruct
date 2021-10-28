#pragma once
#include <string>

namespace wnet {

	static const uint8_t format_time_buffer_size = sizeof("xxxx-xx-xx xx:xx:xx:xxx");

	enum FormatTimeUnit {
		FTU_YEAR		= 1,
		FTU_MONTH		= 2,
		FTU_DAY			= 3,
		FTU_HOUR		= 4,
		FTU_MINUTE		= 5,
		FTU_SECOND		= 6,
		FTU_MILLISECOND	= 7,
	};
	//default time string [xxxx-xx-xx xx:xx:xxx]
	std::string GetFormatTime(FormatTimeUnit unit = FTU_MILLISECOND);

	void GetFormatTime(char* buf,uint32_t& len,FormatTimeUnit uint = FTU_MILLISECOND);

	uint64_t UTCTimeSec();
	uint64_t UTCTimeMsec();

	void Sleep(uint32_t interval);
}