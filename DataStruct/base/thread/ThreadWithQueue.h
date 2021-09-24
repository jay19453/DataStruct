#pragma once
#include "Thread.h"
#include "../struct/ThreadSafeQueue.h"

namespace wnet {
	template <typename T>
	class ThreadWithQueue :public Thread
	{
	public:
		ThreadWithQueue() {}
		~ThreadWithQueue() {}
	public:
		void Push(const T& value) {
			_queue.push(value);
		}
		
		void Pop(T& value){
			_queue.pop(value);
		}

		int32_t Size()
		{
			return _queue.size();
		}

		virtual void Run() = 0;
	protected:
		ThreadWithQueue(const ThreadWithQueue& thread) = delete;
		ThreadWithQueue& operator=(const ThreadWithQueue&) = delete;
	private:
		ThreadSafeBlockQueue<T> _queue;
	};
}