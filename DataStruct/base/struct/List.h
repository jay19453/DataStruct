#pragma once
#include <memory>

namespace wnet {
	template <typename T>
	class List {
	public:
		List() {}
		~List() {};
	public:
		void clear()
		{
			/*if (!_tail)
			{
				return;
			}
			while (_header)
			{
				auto res = _header;
				_header = _header->getNext();
				res->setNext(nullptr);
				reset(res);
			}*/
			_size = 0;
			_header.reset();
			_tail.reset();
		}

		void pushBack(std::shared_ptr<T> value) {
			if (value.get())
			{
				if (!_tail)
				{
					_header = _tail = value;
				}
				else {
					_tail->setNext(value);
					value->setPrev(_tail);
					_tail = value;
				}
				_size++;
			}
		}
		std::shared_ptr<T> popBack()
		{
			if (!_tail)
			{
				return nullptr ;
			}
			auto res = _tail;
			_tail = _tail->getPrev();
			if (!_tail)
			{
				_header->reset();
			}
			else {
				_tail->setNext(nullptr);
			}
			_size--;
			return res;
		}

		void pushFront(std::shared_ptr<T> value){
			if (value.get())
			{
				if (!_header)
				{
					_header = _tail = value;
				}
				else {
					value->setNext = _header;
					_header->setPrev = value;
					_header = value;
				}
				_size++;
			}
		}

		std::shared_ptr<T> popFront()
		{
			if (!_header)
			{
				return nullptr;
			}
			auto res = _header;
			_header = _header->getNext();
			if (!_header)
			{
				reset(_tail);
			}
			
			_size--;
			return res;
		}

		uint32_t Size() { return _size; }
		std::shared_ptr<T> GetHead() { return _header; }
		std::shared_ptr<T> GetTail() { return _tail; }

	private:
		std::shared_ptr<T> _header;
		std::shared_ptr<T> _tail;
		uint32_t _size = 0;
	};
}