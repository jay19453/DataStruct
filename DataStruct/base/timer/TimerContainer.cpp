#include "TimerContainer.h"
#include "TimerSlot.h"

namespace wnet {
	TimerContainer::TimerContainer(std::shared_ptr<TimerContainer> sub_timer, TIME_UNIT unit, TIME_UNIT max)
		:_sub_timer(sub_timer),_time_unit(unit),_timer_max(max),_size(max / unit),_cur_time(0)
	{
		_bitmap.Init(_size);
	}
	TimerContainer::~TimerContainer()
	{}

	bool TimerContainer::AddTimer(std::weak_ptr<TimerSlot>t, uint32_t time, bool always /*= false*/)
	{
		if (time >= _timer_max)
		{
			return false;
		}

		auto ptr = t.lock();
		if (!ptr)
		{
			return false;
		}

		if (time < _time_unit && _sub_timer)
		{
			return _sub_timer->AddTimer(t, time, always);
		}

		if (!ptr->IsInTimer())
		{
			ptr->SetInterval(time);
			ptr->SetInTimer();
			if (always)
			{
				ptr->SetAlways();
			}
		}
		else {
			return false;
		}
		return InnerAddTimer(ptr, time);
	}

	bool TimerContainer::RemoveTimer(std::weak_ptr<TimerSlot>t)
	{
		
	}

	int32_t TimerContainer::MinTime()
	{
		
	}

	int32_t TimerContainer::CurrentTime()
	{}

	uint32_t TimerContainer::TimerRun(uint32_t step)
	{}

	bool TimerContainer::Empty()
	{}

	void TimerContainer::Clear()
	{}

	bool TimerContainer::InnerAddTimer(std::shared_ptr<TimerSlot> ptr, uint32_t time)
	{
		uint16_t cur_index = time / _time_unit + _cur_time;
	
	}
	uint16_t TimerContainer::TimeUnit2TimeType(TIME_UNIT tu)
	{}
	uint32_t TimerContainer::GetIndexLeftInterval(uint16_t index)
	{}
	void TimerContainer::GetIndexTimer(std::vector<std::weak_ptr<TimerSlot>>& run_timer_slots,
		std::vector<std::weak_ptr<TimerSlot>>& sub_timer_slots, uint32_t index, uint32_t time_pass)
	{}
	void TimerContainer::DoTimer(std::vector<std::weak_ptr<TimerSlot>>& run_timer_slots,
		std::vector<std::weak_ptr<TimerSlot>>& sub_timer_slots)
	{}
}