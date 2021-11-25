#pragma once
#include <memory>
#include <vector>

#include "BufferInterface.h"
#include "../struct/List.h"

namespace wnet {
	class IAlloterWrap;
	class BufferBlock;
	class BlockMemoryPool;
	class BufferQueue :
		public InnerBuffer{
	public:
		BufferQueue(const std::shared_ptr<BlockMemoryPool>& block_pool,
			const std::shared_ptr<IAlloterWrap>& alloter);
		virtual ~BufferQueue();
	public:
		virtual uint32_t Read(char* res, uint32_t len);
		virtual uint32_t Read(std::shared_ptr<InnerBuffer> buffer, uint32_t len = 0);
		virtual uint32_t Write(const char* data, uint32_t len);
		virtual uint32_t Write(std::shared_ptr<InnerBuffer> buffer, uint32_t len = 0);
		virtual uint32_t ReadUntil(char* res, uint32_t len);
		virtual uint32_t ReadUntil(char* res, uint32_t len, const char* find,
			uint32_t find_len, uint32_t& need_len);
		virtual uint32_t ReadNotMovePt(char* res, uint32_t len);
		virtual uint32_t GetCanWriteLength();
		virtual uint32_t GetCanReadLength();
		virtual uint32_t FindStr(const char* s, uint32_t s_len);
		virtual int32_t MoveReadPt(int32_t len);
		virtual int32_t MoveWritePt(int32_t len);
		virtual std::shared_ptr<BlockMemoryPool> GetBlockMemoryPool();
		virtual void Clear();
	public:
		/*virtual uint32_t GetFreeMemoryBlock(std::vector<Iovec>& block_vec, uint32_t size = 0);
		virtual uint32_t GetUseMemoryBlock(std::vector<Iovec>& block_vec, uint32_t max_size = 0);*/
		virtual uint32_t FindStr(const char* s, uint32_t s_len);
		//virtual std::shared_ptr<BlockMemoryPool> GetBlockMemoryPool();
	protected:
		virtual void Reset();
		virtual void Append();
	protected:
		uint32_t _can_read_length;

		List<BufferBlock> _buffer_list;
		std::shared_ptr<BufferBlock> _buffer_write;

		std::shared_ptr<BlockMemoryPool> _block_alloter;
		std::shared_ptr<IAlloterWrap> _alloter;
	};
}