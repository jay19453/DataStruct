#pragma once
#include <stdint.h>
#include <memory>

namespace wnet {
	class BlockMemoryPool;
	class InnerBuffer {
	public:
		InnerBuffer(){}
		virtual ~InnerBuffer(){}

		virtual uint32_t Read(char* res,uint32_t len) = 0;
		virtual uint32_t Read(std::shared_ptr<InnerBuffer> buffer,uint32_t len = 0) = 0;
		virtual uint32_t Write(const char* data,uint32_t len) = 0;
		virtual uint32_t Write(std::shared_ptr<InnerBuffer> buffer,uint32_t len = 0) = 0;
		
		virtual uint32_t ReadUntil(char* res,uint32_t len) = 0;
		virtual uint32_t ReadUntil(char* res,uint32_t len,const char* find,uint32_t find_len, uint32_t& need_len) = 0;

		virtual uint32_t ReadNotMovePt(char* res,uint32_t len) = 0;

		virtual uint32_t GetCanWriteLength() = 0;
		virtual uint32_t GetCanReadLength() = 0;

		virtual uint32_t FindStr(const char* s,uint32_t s_len) = 0;

		virtual int32_t MoveReadPt(int32_t len) = 0;
		virtual int32_t MoveWritePt(int32_t len) = 0;

		virtual std::shared_ptr<BlockMemoryPool> GetBlockMemoryPool() = 0;

		virtual void Clear() = 0;
	};
}