#pragma once

#include <cstdint>
#include <memory>

namespace wnet {

	enum TIME_UNIT :uint32_t {
		TU_MILLISECOND = 1,
		TU_SECOND = TU_MILLISECOND *1000,
		TU_MINUTE = TU_SECOND * 60,
		TU_HOUR	  = TU_MINUTE * 60,
	};

	enum TIMER_CODE {
		NO_TIMER = -1,
	};

	class TimerSlot;
	class Timer {
	public:
		Timer() {}
		~Timer() {}

		virtual bool AddTimer(std::weak_ptr<TimerSlot>t,uint32_t time,bool always = false) = 0;
		virtual bool RemoveTimer(std::weak_ptr<TimerSlot>t) = 0;

		virtual int32_t MinTime() = 0;

		virtual int32_t CurrentTime() = 0;

		virtual uint32_t TimerRun(uint32_t time) = 0;

		virtual bool Empty() = 0;
	};
}