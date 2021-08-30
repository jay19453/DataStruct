#include "PoolAlloter.h"
#include "NormalAlloter.h"

namespace base {
	PoolAlloter::PoolAlloter() {
		_alloter = MakeNormalAlloterPrt();
	}

	PoolAlloter::~PoolAlloter()
	{
		for (int i =0; i < _malloc_vec.size();i++)
		{
			void* data = _malloc_vec[i];
			_alloter->Free(data);
		}
	}

	void* PoolAlloter::Alloc(uint32_t size)
	{
		size = size <= 0 ? __align : size;
		if (size > defaultMaxChunkSize)
		{
			return _alloter->AllocAlign(size);
		}
		int index = freeListIndex(size);
		Node* volatile* myFreeLists = freeLists + index;
		Node* result = *myFreeLists;
		if (!result)
		{
			return ReFill(align(size));
		}
		(*myFreeLists) = result->next_node;
		return (void*)result;
	}

	void* PoolAlloter::AllocZero(uint32_t size)
	{
		void* result = Alloc(size);
		if (result)
		{
			memset(result, 0, size);
		}
		return result;
	}

	void* PoolAlloter::AllocAlign(uint32_t size)
	{
		return AllocAlign(align(size));
	}

	void  PoolAlloter::Free(void*& data, uint32_t len /*= 0*/)
	{
		if (len > defaultMaxChunkSize)
		{
			_alloter->Free(data);
			return;
		}
		int index = freeListIndex(len);
		Node* volatile* myFreeLists = freeLists + index;
		Node* node = (Node*)data;
		node->next_node = *myFreeLists;
		*myFreeLists = node;
	}

	int8_t* PoolAlloter::ReFill(size_t size)
	{
		size_t chunkNum = 20;
		int8_t* result = AllocChunk(size, chunkNum);
		if (!result) return nullptr;
		if (1 == chunkNum) return result;
		//set node nextnode
		Node* volatile* myFreeLists = freeLists + freeListIndex(size);
		Node* start = *myFreeLists = (Node*)(result + size);

		for (int i = 2;;i++)
		{
			if (i == chunkNum)
			{
				start->next_node = nullptr;
				break;
			}
			start->next_node = (Node*)(result + size * i);
			start = start->next_node;
		}
		return result;
	}

	int8_t* PoolAlloter::AllocChunk(size_t size, size_t& n)
	{
		size_t needTotal = size * n;
		size_t leftTotal = _end - _start;
		int8_t* result = _start;
		if (leftTotal >= needTotal)
		{
			_start = _start + needTotal;
			return result;
		}
		else if (leftTotal >= size)
		{
			n = leftTotal / size;
			_start = _start + n * size;
			return result;
		}
		else
		{
			size_t bytes_to_get = 2 * needTotal + align(_size >> 4);
			if (leftTotal > 0)
			{
				Node* volatile* myFreeLists = freeLists + freeListIndex(leftTotal);
				Node* node = (Node*)_start;
				node->next_node = *myFreeLists;
				*myFreeLists = node;
				//_start = _end;
			}
			_start = (int8_t*)_alloter->Alloc(bytes_to_get);
			if (!_start)
			{
				uint32_t index = freeListIndex(size);
				while (true)
				{
					if (index >= defaultNumsOfFreeLists)
					{
						break;
					}
					Node* volatile* myFreeLists = freeLists + index;
					Node* node = *myFreeLists;
					if (node)
					{
						_start = (int8_t*)node;
						_end = _start + size;
						*myFreeLists = node->next_node;
						return AllocChunk(size, n);
					}
					index++;
				}
				return nullptr;
			}
			_malloc_vec.push_back(_start);
			_end = _start + bytes_to_get;
			_size = bytes_to_get;
			return AllocChunk(size, n);
		}
	}
	std::shared_ptr<IAlloter> MakePoolAlloterPtr() {
		return std::make_shared<PoolAlloter>();
	}
}