#pragma once
#include <vector>
//是对bit进行操作包括insert和find操作。
//其中find和linux内核find_next_bit方法差不多
//这个方法详细可查看https://www.cnblogs.com/zpcdbky/p/5825849.html
namespace wnet {
	class Bitmap {
	public:
		Bitmap();
		~Bitmap();
	public:
		bool Init(uint32_t size);
		bool Insert(uint32_t index);
		bool Remove(uint32_t index);
		int32_t Find(uint32_t index);
		bool Empty();
		void Clear();
	private:
		uint32_t _vec_index;
		std::vector<uint64_t> _bitmap;
	};
}