#pragma once
#include <vector>
#include <memory>

namespace wnet {
	class BlockMemoryPool {
	public:
		BlockMemoryPool(size_t size,size_t num);
		~BlockMemoryPool();
	public:
		void* AllocBlock();
		void FreeBlock(void*& data);
		void ReleaseHalf();

		void Expansion(size_t num = 0);
		uint32_t VecSize();
		uint32_t GetBlockLength();
	private:
		size_t block_num;
		size_t block_size;
		std::vector<void*> block_vec;
	};
	std::shared_ptr<BlockMemoryPool> MakeBlockMemPool(size_t size, size_t num);
}