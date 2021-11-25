#include "BlockMemoryPool.h"
#include "../public.h"
#include "../../config/NetConfig.h"

namespace wnet {

	BlockMemoryPool::BlockMemoryPool(size_t size, size_t num):block_size(size),block_num(num)
	{
		block_size = _ALIGN(block_size, __align);
	}

	BlockMemoryPool::~BlockMemoryPool()
	{
		for (int i = 0; i < block_vec.size();i++)
		{
			free(block_vec[i]);
		}
		block_vec.clear();
	}

	void* BlockMemoryPool::AllocBlock()
	{
		if (!block_vec.empty())
		{
			void* res = block_vec.back();
			block_vec.pop_back();
			return res;
		}
		else {
			Expansion();
			return AllocBlock();
		}
	}

	void BlockMemoryPool::FreeBlock(void*& data)
	{
		if (nullptr == data)
		{
			return;
		}
		if (block_vec.size() > max_block_nums)
		{
			ReleaseHalf();
		}
		block_vec.push_back(data);
	}

	void BlockMemoryPool::Expansion(size_t num)
	{
		if (0 == num)
		{
			num = block_num;
		}
		for (int i = 0; i < num; i++)
		{
			void* res = malloc(block_size);
			block_vec.push_back(res);
		}
	}

	void BlockMemoryPool::ReleaseHalf()
	{
		size_t delnum = block_vec.size() - block_vec.size() / 2;
		for (int i = 0;i < delnum;i++)
		{
			void* res = block_vec.back();
			block_vec.pop_back();
			free(res);
		}
	}

	uint32_t BlockMemoryPool::VecSize()
	{
		return block_vec.size();
	}

	uint32_t BlockMemoryPool::GetBlockLength()
	{
		return block_size;
	}

	std::shared_ptr<BlockMemoryPool> MakeBlockMemPool(size_t size, size_t num)
	{
		return std::make_shared<BlockMemoryPool>(size,num);
	}
}