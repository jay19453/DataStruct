#include "BufferQueue.h"
#include "BufferBlock.h"
#include "../alloter/AlloterInterface.h"
#include "../alloter/BlockMemoryPool.h"

namespace wnet {

	BufferQueue::BufferQueue(const std::shared_ptr<BlockMemoryPool>& block_pool, 
		const std::shared_ptr<IAlloterWrap>& alloter)
		:_block_alloter(block_pool),
		_alloter(alloter)
	{
		_can_read_length = 0;
	}

	BufferQueue::~BufferQueue()
	{
		_buffer_list.clear();
	}

	void BufferQueue::Clear()
	{
		Reset();
		_can_read_length = 0;
	}

	std::shared_ptr<BlockMemoryPool> BufferQueue::GetBlockMemoryPool()
	{
		return _block_alloter;
	}

	//buffer is BufferQueue class
	uint32_t BufferQueue::Read(std::shared_ptr<InnerBuffer> buffer , uint32_t len/* = 0*/)
	{
		if (_buffer_list.Size() == 0 || !buffer)
		{
			return 0;
		}
		if (len == 0)
		{
			len = GetCanReadLength();
		}
		if (len == 0)
		{
			return 0;
		}
		std::shared_ptr<BufferQueue> buffer_queue = std::dynamic_pointer_cast<BufferQueue> (buffer);
		if (!buffer_queue->_buffer_write)
		{
			buffer_queue->Append();
		}
		
		uint32_t can_write_len = buffer_queue->_buffer_write->GetCanWriteLength();
		uint32_t total_read_len = 0;
		uint32_t cur_read_len = 0;
		auto read_buffer = _buffer_list.GetHead();

		while (1)
		{
			cur_read_len = read_buffer->Read(buffer_queue->_buffer_write, can_write_len);
			total_read_len += cur_read_len;
			// current write block is full
			if (cur_read_len >= can_write_len)
			{
				if (total_read_len >= len)
				{
					break;
				}
				buffer_queue->Append();
				buffer_queue->_buffer_write = buffer_queue->_buffer_list.GetTail();
				can_write_len = buffer_queue->_buffer_write->GetCanWriteLength();
			}
			else {
				can_write_len -= cur_read_len; 
				if (read_buffer == _buffer_write)
				{
					if (_buffer_write->getNext())
					{
						_buffer_write = _buffer_write->getNext();
					}
					else {
						Reset();
						break;
					}
				}
				_buffer_list.popFront();
				read_buffer = _buffer_list.GetHead();
			}

			if (total_read_len >= len)
			{
				break;
			}
		}
		_can_read_length -= total_read_len;
		buffer_queue->_can_read_length += total_read_len;
		return total_read_len;
	}

	uint32_t BufferQueue::Write(std::shared_ptr<InnerBuffer> buffer, uint32_t len /*= 0*/)
	{
		if (!buffer)
		{
			return 0;
		}

		if (len == 0)
		{
			len = buffer->GetCanReadLength();
		}
		if (len == 0)
		{
			return 0;
		}

		auto buffer_queue = std::dynamic_pointer_cast<BufferQueue>(buffer);
		uint32_t total_write_size = 0;
		uint32_t cur_write_size = 0;
		uint32_t should_write_size = buffer_queue->_buffer_list.GetHead()->GetCanReadLength();
		auto buffer_list = buffer_queue->_buffer_list;
		auto buffer_read = buffer_queue->_buffer_list.GetHead();

		while (1)
		{
			if (!_buffer_write)
			{
				Append();
				_buffer_write = _buffer_list.GetTail();
			}

			cur_write_size = _buffer_write->Write(buffer_read, should_write_size);
			total_write_size += cur_write_size;
			if (cur_write_size >= should_write_size)
			{
				if (buffer_read == buffer_queue->_buffer_write)
				{
					if (buffer_queue->_buffer_write->getNext())
					{
						buffer_queue->_buffer_write = buffer_queue->_buffer_write->getNext();
					}
					else {
						buffer_queue->Reset();
						break;
					}
				}
				buffer_list.popFront();
				buffer_read = buffer_list.GetHead();
				should_write_size = buffer_read->GetCanReadLength();
			}
			else {
				if (total_write_size >= len)
				{
					break;
				}
				should_write_size -= cur_write_size;
				_buffer_write = _buffer_write->getNext();
			}

			if (should_write_size >= len)
			{
				break;
			}
		}
		_can_read_length += total_write_size;
		buffer_queue->_can_read_length -= total_write_size;
		return total_write_size;
	}

	uint32_t BufferQueue::Read(char* res, uint32_t len)
	{
		if (_buffer_list.Size() == 0 || !res || len == 0)
		{
			return 0;
		}

		auto buffer_read  = _buffer_list.GetHead();
		uint32_t read_done_size = 0;
		while (buffer_read)
		{
			read_done_size += buffer_read->Read(res + read_done_size, len - read_done_size);
			if (read_done_size >= len)
			{
				break;
			}
			//说明write 已经写完buffer 的时候判断是否存在bufferblock
			if (buffer_read == _buffer_write)
			{
				if (_buffer_write->getNext())
				{
					_buffer_write = _buffer_write->getNext();
				}
				else {
					Reset();
					break;
				};
			}
			_buffer_list.popFront();
			buffer_read = _buffer_list.GetHead();
		}
		_can_read_length -= read_done_size;
		return read_done_size;
	}

	uint32_t BufferQueue::Write(const char* data, uint32_t len)
	{
		if (len == 0 || !data)
		{
			return 0;
		}
		
		uint32_t done_write_size = 0;
		while (1)
		{
			if (!_buffer_write)
			{
				Append();
				_buffer_write = _buffer_list.GetTail();
			}
			if (!_buffer_write)
			{
				break;
			}
			done_write_size += _buffer_write->Write(data, len);
			if (done_write_size >= len)
			{
				break;
			}
			_buffer_write = _buffer_write->getNext();
		}
		_can_read_length += done_write_size;
		return done_write_size;
	}

	int32_t  BufferQueue::MoveReadPt(int32_t len)
	{
		if (len == 0 || _buffer_list.Size() == 0)
		{
			return 0;
		}

		auto read_buffer = _buffer_list.GetHead();
		
int32_t total_read_len = 0;

		if (len > 0)
		{
			while (read_buffer)
			{
				total_read_len += read_buffer->MoveReadPt(len - total_read_len);
				if (total_read_len >= len)
				{
					break;
				}
				if (read_buffer == _buffer_write)
				{
					if (_buffer_write->getNext())
					{
						_buffer_write = _buffer_write->getNext();
					}
					else {
						Reset();
						break;
					}
				}
				_buffer_list.popFront();
				read_buffer = _buffer_list.GetHead();
			}
		}
		else {
			total_read_len += read_buffer->MoveReadPt(len);
		}

		_can_read_length -= total_read_len;
		return total_read_len;
	}

	int32_t	 BufferQueue::MoveWritePt(int32_t len)
	{
		if (len == 0)
		{
			return 0;
		}

		int32_t total_write_len = 0;
		if (len > 0)
		{
			while (_buffer_write)
			{
				total_write_len += _buffer_write->MoveWritePt(len - total_write_len);
				if (total_write_len >= len || _buffer_write == _buffer_list.GetTail())
				{
					break;
				}
				_buffer_write = _buffer_write->getNext();
			}
		}
		else {
			while (_buffer_write)
			{
				total_write_len += _buffer_write->MoveWritePt(len + total_write_len);
				if (total_write_len >= -
len || _buffer_write == _buffer_list.GetHead())
				{
					break;
				}
				_buffer_write = _buffer_write->getPrev();
			}
		}
		_can_read_length += total_write_len;
		return total_write_len;
	}

	uint32_t BufferQueue::ReadNotMovePt(char* res, uint32_t len)
	{
		if (_buffer_list.Size() == 0 || !res || len == 0)
		{
			return 0;
		}
		auto buffer_read = _buffer_list.GetHead();
		uint32_t total_read_size = 0;
		while (buffer_read)
		{
			total_read_size += buffer_read->ReadNotMovePt(res + total_read_size, len - total_read_size);
			if (total_read_size >= len)
			{
				break;
			}
			if (buffer_read == _buffer_write)
			{
				break;
			}
			buffer_read = buffer_read->getNext();
		}
		return total_read_size;
	}

	uint32_t BufferQueue::ReadUntil(char* res, uint32_t len)
	{
		if (GetCanReadLength() < len)
		{
			return 0;
		}

		return Read(res, len);
	}

	uint32_t BufferQueue::ReadUntil(char* res, uint32_t len, const char* find,
		uint32_t find_len, uint32_t& need_len)
	{
		uint32_t size = FindStr(find, find_len);
		if (size)
		{
			if (size >= len)
			{
				return Read(res, len);
			}
			else {
				need_len = size;
				return 0;
			}
		}
		return 0;
	}

	uint32_t BufferQueue::GetCanWriteLength()
	{
		uint32_t write_len = 0;
		std::shared_ptr<BufferBlock> tmp = _buffer_write;
		while (tmp)
		{
			write_len += tmp->GetCanWriteLength();
			if (tmp == _buffer_list.GetTail())
			{
				break;
			}
			tmp = tmp->getNext();
		}
		return write_len;
	}

	uint32_t BufferQueue::GetCanReadLength()
	{
		return _can_read_length;
	}

	uint32_t BufferQueue::FindStr(const char* s, uint32_t s_len)
	{
		if (_buffer_list.Size() == 0)
		{
			return 0;
		}
		uint32_t res = 0;
		auto read_buffer = _buffer_list.GetHead();
		uint32_t total_len = 0;
		while (read_buffer)
		{
			res = read_buffer->FindStr(s, s_len);
			if (res)
			{
				total_len =+ res;
				break;
			}
			if (read_buffer == _buffer_write)
			{
				break;
			}
			total_len += read_buffer->GetCanReadLength();
			read_buffer = read_buffer->getNext();
		}
		return total_len;
	}

	void BufferQueue::Reset()
	{
		_buffer_list.clear();
		_buffer_write.reset();
	}

	void BufferQueue::Append()
	{
		auto temp = _alloter->PoolNewSharePtr<BufferBlock>(_block_alloter);
		if (!_buffer_write)
		{
			_buffer_write = temp;
		}
		_buffer_list.pushBack(temp);
	}
}