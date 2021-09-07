#pragma once
#include <vector>
#include <memory>

namespace base {
	class BlockMemoryPool {
	public:
		BlockMemoryPool(size_t size,size_t num);
		~BlockMemoryPool();
	public:
		void* AllocBlock();
		void FreeBlock(int8_t* data);
		void ReleaseHalf();

		void Expansion(size_t num = 0);
		uint32_t VecSize();

	private:
		size_t block_num;
		size_t block_size;
		std::vector<void*> block_vec;
	};
	std::shared_ptr<BlockMemoryPool> MakeBlockMemPool(size_t size, size_t num);
}