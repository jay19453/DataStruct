#include "TimerSlot.h"

namespace wnet {
	TimerSlot::TimerSlot(){
		_left_interval = 0;
		_total_interval = 0;
		_cur_index = 0;
	}

	TimerSlot::~TimerSlot() {

	}

	void TimerSlot::SetInterval(uint32_t interval)
	{
		_total_interval = interval;
		_left_interval = interval;
	}

	uint32_t TimerSlot::GetTotalInterval()
	{
		_total_interval &= ~(TRT_ALWAYS | TRT_IN_TIMER);
		return _total_interval;
	}

	uint32_t TimerSlot::GetLeftInterval()
	{
		return _left_interval;
	}

	//把left interval 和total interval 一样
	//为了always周期执行
	void TimerSlot::ResetTime()
	{
		_left_interval = _total_interval;
		_cur_index = 0;
	}

	uint32_t TimerSlot::TimePass(uint32_t time)
	{
		_left_interval -= time;
		return _left_interval;
	}

	void TimerSlot::SetInTimer() {
		_total_interval |= TRT_IN_TIMER;
	}
	bool TimerSlot::IsInTimer() {
		return _total_interval & TRT_IN_TIMER;
	}
	void TimerSlot::RemoveInTimer() {
		_total_interval &= ~TRT_IN_TIMER;
	}

	void TimerSlot::SetAlways()
	{
		_total_interval |= TRT_ALWAYS;
	}

	bool TimerSlot::IsAlways() {
		return _total_interval & TRT_ALWAYS;
	}
	void TimerSlot::RemoveAlways() {
		_total_interval &= ~TRT_ALWAYS;
	}

	void TimerSlot::SetCurIndex(uint16_t index, uint16_t type)
	{
		_cur_index = index | type;
	}
	void TimerSlot::GetCurIndex(uint16_t& index, uint16_t& type) {
		type = _cur_index & TIT_MUSK;
		index = _cur_index & ~TIT_MUSK;
	}
}