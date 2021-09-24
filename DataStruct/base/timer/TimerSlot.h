#pragma once
#include <stdint.h>

namespace wnet {
	enum TIMER_INDEX_TYPE:uint16_t
	{
		TIT_MILLISECOND = 0x01 << 10,
		TIT_SECOND = 0x02 << 10,
		TIT_MINUTE = 0x03 << 10,
		TIT_MUSK = 0x07 << 10,
	};
	//时间轮中时间槽 类
	class TimerSlot {
	public:
		TimerSlot();
		~TimerSlot();
	public:
		enum TIMER_RUN_TYPE :uint32_t {
			TRT_IN_TIMER = (uint32_t)1 << 30,
			TRT_ALWAYS = (uint32_t)1 << 31
		};

		//callback when time out
		virtual void OnTimer() = 0;

		void SetInterval(uint32_t interval);
		uint32_t GetTotalInterval();
		uint32_t GetLeftInterval();

		void ResetTime();
		uint32_t TimePass(uint32_t time);
		
		void SetInTimer();
		bool IsInTimer();
		void RemoveInTimer();

		void SetAlways();
		bool IsAlways();
		void RemoveAlways();

		void SetCurIndex(uint16_t index, uint16_t type);
		void GetCurIndex(uint16_t& index, uint16_t& type);
	private:
		uint16_t _cur_index;
		uint32_t _total_interval;
		uint32_t _left_interval;
	};
}