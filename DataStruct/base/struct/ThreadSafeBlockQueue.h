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

		T pop()
		{
			lock_guard<std::mutex> lock(_mutex);
			cv.wait(_mutex, [this]()->bool {return !_queue.empty(); });
			auto res = std::move(_queue.front());
			_queue.pop();
			return res;
		}

		void clear()
		{
			lock_guard<std::mutex> lock(_mutex);
			while (!_queue.empty())
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