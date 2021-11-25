#pragma once
#include "BufferInterface.h"
#include "../struct/ListSlot.h"

namespace wnet {

	class BlockMemoryPool;
	class BufferBlock :
		public InnerBuffer,
		public ListSlot<BufferBlock> {
	public:
		BufferBlock(std::shared_ptr<BlockMemoryPool>& alloter);
		~BufferBlock();

		uint32_t ReadNotMovePt(char* res, uint32_t len);

		uint32_t Read(std::shared_ptr<InnerBuffer> buffer, uint32_t len /* = 0 */);
		uint32_t Write(std::shared_ptr<InnerBuffer> buffer, uint32_t len /* = 0 */);

		uint32_t Read(char* res, uint32_t len);
		uint32_t Write(const char* data, uint32_t len);

		void Clear();

		int32_t MoveReadPt(int32_t len);
		int32_t MoveWritePt(int32_t len);

		uint32_t ReadUntil(char* res, uint32_t len);
		// do not read when can't find specified character.
		// return read bytes when read otherwise return 0
		// when find specified character but res length is too short, 
		// return 0 and the last param return need length
		uint32_t ReadUntil(char* res, uint32_t len, const char* find, uint32_t find_len, uint32_t& need_len);

		uint32_t GetCanReadLength();
		uint32_t GetCanWriteLength();

		bool GetFreeMemoryBlock(void*& res1, uint32_t& len1, void*& res2, uint32_t& len2);
		bool GetUseMemoryBlock(void*& res1, uint32_t& len1, void*& res2, uint32_t& len2);

		uint32_t FindStr(const char* s, uint32_t s_len);

		std::shared_ptr<BlockMemoryPool> GetBlockMemoryPool();
	private:
		const char* _FindStrInMem(const char* buffer,const char* ch, uint32_t buffer_len, uint32_t ch_len);
		uint32_t _Read(char* res,uint32_t len,bool move_pt);
		uint32_t _Write(const char* data,uint32_t len);
	private:
		uint32_t _total_size;    //total buffer size
		char*  _read;			 //read position
		char*  _write;			 //write position
		char*  _buffer_start;
		char*  _buffer_end;
		bool   _can_read;
		std::weak_ptr<BlockMemoryPool> _alloter;
	};
}
