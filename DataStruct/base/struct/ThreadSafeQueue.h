#pragma once
//queue FIFO rules 
#include <memory>
#include <queue>
#include <mutex>

namespace wnet {
	template <typename T>
	class SafeQueue {
	public:
		SafeQueue() {}
		~SafeQueue() {}
	public:
		bool empty()
		{
			std::lock_guard<std::mutex> lock(_mutex);
			return _queue.empty();
		}

		void push(const T& value) {
			std::lock_guard<std::mutex> lock(_mutex);
			_queue.push(T);
		}
		bool pop(T& value)
		{
			std::lock_guard<std::mutex> lock(_mutex);
			if (_queue.empty())
			{
				return false;
			}
			value = std::move(_queue.front());
			_queue.pop();
			return true;
		}
		void clear()
		{
			std::lock_guard<std::mutex> lock(_mutex);
			while (_queue.empty())
			{
				_queue.pop();
			}
		}

		int32_t size()
		{
			lock_guard<std::mutex> lock(_mutex);
			return _queue.size();
		}
	private:
		std::queue<T> _queue;
		std::mutex _mutex;
	};
}