#pragma once
#include "AlloterInterface.h"
#include <vector>

namespace wnet {
	static const uint8_t  defaultMaxChunkSize = 128;
	static const uint8_t  defaultNumsOfFreeLists = defaultMaxChunkSize / __align;
	static const uint8_t  default_number_add_nodes = 20;

	class PoolAlloter :public IAlloter
	{
	public:
		PoolAlloter();
		~PoolAlloter();
	public:
		void* Alloc(uint32_t size);
		void* AllocZero(uint32_t size);
		void* AllocAlign(uint32_t size);

		void  Free(void*& data, uint32_t len = 0);
	private:
		int8_t* ReFill(size_t size,uint32_t num = default_number_add_nodes);
		int8_t* AllocChunk(size_t size, size_t& n);
	private:
		uint32_t freeListIndex(size_t size)
		{
			return (size + __align - 1) / __align - 1;
		}
	private:
		union Node
		{
			union Node* next_node;
			char data[1];
		};
	private:
		Node* volatile freeLists[defaultNumsOfFreeLists] = {0};
		std::vector<int8_t*> _malloc_vec;
	private:
		int8_t* _start;
		int8_t* _end;
		uint32_t _size;
		std::shared_ptr<IAlloter> _alloter;
	};

	std::shared_ptr<IAlloter> MakePoolAlloterPtr();
}