#include "Bitmap.h"

namespace wnet {
	static const uint64_t step_base = 1;
	static const uint32_t step_size = sizeof(uint64_t) * 8;

	Bitmap::Bitmap()
	{
		_vec_index = 0;
	}

	Bitmap::~Bitmap()
	{}

	bool Bitmap::Init(uint32_t size)
	{
		uint32_t vec_size = size / step_size;
		if (vec_size > sizeof(_vec_index)*8)
		{
			return false;
		}
		if (size % step_size > 0)
		{
			vec_size++;
		}

		_bitmap.resize(vec_size);
		for (std::size_t i = 0; i < _bitmap.size(); i++) {
			_bitmap[i] = 0;
		}
		return true;
	}

	bool Bitmap::Insert(uint32_t index)
	{
		if (index > _bitmap.size() * step_size)
		{
			return false;
		}
		uint32_t _index_r = index / step_size;
		uint32_t _index_c = index % step_size;
		_bitmap[_index_r] |= step_base << _index_c;
		_vec_index |= step_base << _index_r;
		return true;
	}

	bool Bitmap::Remove(uint32_t index)
	{
		if (index > _bitmap.size() * step_size)
		{
			return false;
		}
		uint32_t _index_r = index / step_size;
		uint32_t _index_c = index % step_size;
		_bitmap[_index_r] &= ~(step_base << _index_c);
		_vec_index &= ~(step_base << _index_r);
		return true;
	}

	int32_t Bitmap::Find(uint32_t index)
	{
		if (index >= _bitmap.size() * step_size || Empty())
		{
			return -1;
		}
		uint32_t _index_r = index / step_size;
		//uint32_t _index_c = index % step_size;
		uint32_t ret = _index_r * step_size;

		if (_bitmap[_index_r] != 0)
		{
			int32_t cur_tep = index - ret;
			int64_t cur_value = _bitmap[_index_r] >> cur_tep;
			if (cur_value != 0)
			{
				ret += std::log2f(float(cur_value & (-cur_value))) + cur_tep;
				return ret;
			}
			else ret += step_size;
		}
		else ret += step_size;

		int32_t _vec_temp_index = _vec_index >> _index_r;
		if (_vec_temp_index == 0)
		{
			return -1;
		}
		uint32_t cur_index = (uint32_t)std::log2f(float(_vec_temp_index & (-_vec_temp_index))) + 1;
		uint32_t target_index = _index_r + cur_index;

		int64_t target_value = _bitmap[target_index];
		ret += (target_index - 1) * step_size;
		ret += (uint32_t)std::log2f(float(target_value & (-target_value)));
		return ret;

		/*while (_index_c < step_size)
		{
			++_index_c;
			if (_bitmap[_index_r] & (step_base << _index_c))
			{
				return index + _index_c;
			}
		}*/
		return -1;
	}

	bool Bitmap::Empty()
	{
		return _vec_index == 0;
	}
	void Bitmap::Clear()
	{
		while (_vec_index != 0) {
			int32_t next_vec_index = (int32_t)std::log2f(float(_vec_index & (-(int32_t)_vec_index)));
			_bitmap[next_vec_index] = 0;
			_vec_index = _vec_index & (_vec_index - 1);
		}
	}
}

