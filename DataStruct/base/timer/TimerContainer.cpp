#include <algorithm>
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
		if (time > _timer_max)
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

	bool TimerContainer::InnerAddTimer(std::shared_ptr<TimerSlot> ptr, uint32_t time)
	{
		uint32_t cur_index = time / _time_unit + _cur_time;
		uint32_t left_time = time % _time_unit;

		if (cur_index >= _size)
		{
			cur_index -= _size;
		}

		if (!_sub_timer)
		{
			left_time = 0;
		}

		ptr->SetCurIndex(cur_index, TimeUnit2TimeType(_time_unit));
		ptr->TimePass(time - left_time);

		_timer_wheel[cur_index][left_time].push_back(ptr);
		return _bitmap.Insert(cur_index);
	}

	bool TimerContainer::RemoveTimer(std::weak_ptr<TimerSlot>t)
	{
		auto ptr = t.lock();
		if (!ptr)
		{
			return false;
		}
		uint16_t cur_index = 0;
		uint16_t type =0;
		ptr->GetCurIndex(cur_index,type);

		if (type == TimeUnit2TimeType(_time_unit))
		{
			uint16_t left_time = ptr->GetLeftInterval();
			ptr->RemoveInTimer();
			_bitmap.Remove(cur_index);

			auto timer_map = _timer_wheel.find(cur_index);
			if (timer_map == _timer_wheel.end())
			{
				return true;
			}
			if (!_sub_timer)
			{
				left_time = 0;
			}

			auto sub_map = timer_map->second.find(left_time);
			if (sub_map == timer_map->second.end())
			{
				return false;
			}

			for (auto itor = sub_map->second.begin();itor != sub_map->second.end(); ++itor)
			{
				if (itor->lock() && itor->lock() == ptr)
				{
					sub_map->second.erase(itor);
					return true;
				}
			}
		}

		if (_sub_timer)
		{
			return _sub_timer->RemoveTimer(ptr);
		}
		return false;
	}

	int32_t TimerContainer::MinTime()
	{
		int32_t local_time = LocalMinTime();
		int32_t pass_time = 0;
		int32_t sub_time = 0;

		if (_sub_timer)
		{
			sub_time = _sub_timer->MinTime();
			pass_time = _sub_timer->CurrentTime();
			if (sub_time > 0)
			{
				local_time -= pass_time;
				if (local_time > 0)
				{
					return std::min(sub_time,local_time);
				}
				return sub_time;
			}
			else if (local_time > 0) {
				return local_time - pass_time;
			}
		}
		return local_time;
	}

	int32_t TimerContainer::CurrentTime()
	{
		int32_t time = _cur_time * _time_unit;
		if (_sub_timer)
		{
			time += _sub_timer->CurrentTime();
		}
		return time;
	}

	int32_t TimerContainer::LocalMinTime()
	{
		int32_t next_time = _bitmap.Find(_cur_time);
		if (next_time >= 0)
		{
			return (next_time - _cur_time ) * _time_unit + GetIndexLeftInterval(next_time);
		}

		if (_cur_time > 0)
		{
			next_time = _bitmap.Find(0);
			if (next_time >= 0)
			{
				return (next_time - _cur_time) * _time_unit + GetIndexLeftInterval(next_time);
			}
		}
		return NO_TIMER;
	}

	uint32_t TimerContainer::TimerRun(uint32_t time)
	{
		uint32_t time_pass = time / _time_unit;
		uint32_t time_left = time % _time_unit;

		bool do_timer = time_pass > 0;
		if (time_left > 0 && _sub_timer)
		{
			uint32_t ret = _sub_timer->TimerRun(time_left);
			_cur_time += ret;
			if (ret > 0)
			{
				do_timer = true;
			}
		}

		if (!do_timer)
		{
			return;
		}

		uint32_t pre_time = _cur_time;
		_cur_time += time_pass;

		std::vector<std::weak_ptr<TimerSlot>> run_timer_slots;
		std::vector<std::weak_ptr<TimerSlot>> sub_timer_slots;
		while (1)
		{
			int32_t next_time = _bitmap.Find(pre_time);
			if (next_time < 0) break;
			if (next_time > _cur_time) break;
			GetIndexTimer(run_timer_slots, sub_timer_slots, next_time, time_left);
			pre_time = next_time + 1;
		}

		uint32_t step = 0;
		if (_cur_time >= _size)
		{
			_cur_time -= _size;
			step++;
		}

		if (step > 0)
		{
			pre_time = 0;
			while (1)
			{
				int32_t next_time = _bitmap.Find(pre_time);
				if (next_time < 0) break;
				if (next_time > _cur_time) break;
				GetIndexTimer(run_timer_slots, sub_timer_slots, next_time, time_left);
				pre_time = next_time + 1;
			}
		}

		DoTimer(run_timer_slots, sub_timer_slots);
		return step;
	}

	void TimerContainer::GetIndexTimer(std::vector<std::weak_ptr<TimerSlot>>& run_timer_slots,
		std::vector<std::weak_ptr<TimerSlot>>& sub_timer_slots, uint32_t index, uint32_t time_pass)
	{
		std::unordered_map<uint32_t, std::map<uint32_t, std::list<std::weak_ptr<TimerSlot>>>>::iterator bucket_iter =
			_timer_wheel.find(index);
		if (bucket_iter == _timer_wheel.end())
		{
			return;
		}

		std::map<uint32_t, std::list<std::weak_ptr<TimerSlot>>>& bucket = bucket_iter->second;
		
		for (auto timer_list = bucket.begin();timer_list != bucket.end();timer_list++)
		{
			//这是最低级的时间轮情况
			if (timer_list->first == 0)
			{
				run_timer_slots.insert(run_timer_slots.end(),timer_list->second.begin(),timer_list->second.end());
				continue;
			}
			//存在有下一级别的时间轮的情况下需要执行的timer
			if (timer_list->first <= time_pass)
			{
				run_timer_slots.insert(run_timer_slots.end(),timer_list->second.begin(),timer_list->second.end());
				continue;
			}
			//存在有下一级别的时间轮的情况下 还没有到达执行的timer
			for (auto iter = timer_list->second.begin();iter != timer_list->second.end(); ++iter)
			{
				auto timer = iter->lock();
				sub_timer_slots.push_back(timer);
			}
		}

		_bitmap.Remove(index);
		_timer_wheel.erase(bucket_iter);
	}

	void TimerContainer::DoTimer(std::vector<std::weak_ptr<TimerSlot>>& run_timer_slots,
		std::vector<std::weak_ptr<TimerSlot>>& sub_timer_slots)
	{
		for (auto iter = run_timer_slots.begin();iter != run_timer_slots.end();iter++)
		{
			auto slot = iter->lock();
			if (!slot) continue;
			slot->OnTimer();
			slot->RemoveInTimer();

			if (slot->IsAlways())
			{
				slot->ResetTime();
				auto root_timer = _root_timer.lock();
				if (root_timer)
				{
					root_timer->AddTimer(slot,slot->GetTotalInterval(),slot->IsAlways());
				}
				else {
					AddTimer(slot, slot->GetTotalInterval(),slot->IsAlways());
				}
			}
		}

		if (!_sub_timer)
		{
			return;
		}

		for (auto itor = sub_timer_slots.begin();itor != sub_timer_slots.end();itor++)
		{
			auto slot = itor->lock();
			if (!slot) continue;
			AddTimer(slot, slot->GetLeftInterval(), slot->IsAlways());
		}
	}

	bool TimerContainer::Empty()
	{
		return _bitmap.Empty();
	}

	void TimerContainer::Clear()
	{
		_bitmap.Clear();
		_timer_wheel.clear();
		if (_sub_timer)
		{
			_sub_timer->Clear();
		}
	}

	uint16_t TimerContainer::TimeUnit2TimeType(TIME_UNIT tu)
	{
		uint16_t type = 0;
		switch (tu)
		{
		case wnet::TU_MILLISECOND:
			type = TIT_MILLISECOND;
			break;
		case wnet::TU_SECOND:
			type = TIT_SECOND;
			break;
		case wnet::TU_MINUTE:
			type = TIT_MINUTE;
			break;
		default:
			throw "invalid time unit";
		}
		return type;
	}
	uint32_t TimerContainer::GetIndexLeftInterval(uint16_t index)
	{
		uint32_t left_interval = 0;
		auto timer_map = _timer_wheel.find(index);
		if (timer_map != _timer_wheel.end() && timer_map->second.size() > 0)
		{
			left_interval = timer_map->second.begin()->first;
		}
		return left_interval;
	}
}