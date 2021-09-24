#pragma once
#include <queue>
#include <condition_variable>

namespace wnet {
	template <typename T>
	class ThreadSafeBlockQueue {
	public:
		ThreadSafeBlockQueue() {}
		~ThreadSafeBlockQueue() {}
	public:
		void push(const T& value) {
			lock_guard<std::mutex> lock(_mutex);
			_queue.push(T);
			cv.notify_all();
		}

		bool pop(T& value)
		{
			lock_guard<std::mutex> lock(_mutex);
			if (_queue.empty())
			{
				cv.wait(_mutex);
			}
			value = std::move(_queue.front(), [this]()->bool {return !_queue.empty(); });
			_queue.pop();
			return true;
		}

		void clear()
		{
			lock_guard<std::mutex> lock(_mutex);
			while (_queue.empty())
			{
				_queue.pop();
			}
		}
		bool empty()
		{
			lock_guard<std::mutex> lock(_mutex);
			return _queue.empty();
		}

		int32_t size()
		{
			lock_guard<std::mutex> lock(_mutex);
			return _queue.size();
		}
	private:
		std::queue<T> _queue;
		std::mutex _mutex;
		std::condition_variable_any cv;
	};
}