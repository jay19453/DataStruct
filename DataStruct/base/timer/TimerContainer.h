#pragma once

#include <unordered_map>
#include <map>
#include <list>
#include "TimerInterface.h"
#include "../util/Bitmap.h"

namespace wnet {

	class TimerContainer : public Timer {
	public:
		TimerContainer(std::shared_ptr<TimerContainer> sub_timer,TIME_UNIT unit,TIME_UNIT max);
		~TimerContainer();
	public:
		bool AddTimer(std::weak_ptr<TimerSlot>t, uint32_t time, bool always  = false );
		bool RemoveTimer(std::weak_ptr<TimerSlot>t);

		int32_t MinTime();
		int32_t CurrentTime();

		uint32_t TimerRun(uint32_t step);

		bool Empty();
		void Clear();

		void SetRootTimer(std::shared_ptr<TimerContainer> timer) { _root_timer = timer; }
	protected:
		bool InnerAddTimer(std::shared_ptr<TimerSlot> ptr,uint32_t time);
		uint16_t TimeUnit2TimeType(TIME_UNIT tu);
		uint32_t GetIndexLeftInterval(uint16_t index);
		void GetIndexTimer(std::vector<std::weak_ptr<TimerSlot>>& run_timer_slots,
			std::vector<std::weak_ptr<TimerSlot>>& sub_timer_slots,uint32_t index,uint32_t time_pass);
		void DoTimer(std::vector<std::weak_ptr<TimerSlot>>& run_timer_slots,
			std::vector<std::weak_ptr<TimerSlot>>& sub_timer_slots);

	protected:
		TIME_UNIT _time_unit;
		uint32_t _size;
		uint32_t _timer_max;
	
		uint32_t _cur_time;
		Bitmap	 _bitmap;
		std::weak_ptr<TimerContainer> _root_timer;
		std::shared_ptr<TimerContainer> _sub_timer;
		std::unordered_map<uint32_t,std::map<uint32_t,std::list<std::weak_ptr<TimerSlot>>>> _timer_wheel;
	};
}