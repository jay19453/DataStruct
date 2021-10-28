#pragma once
#include <thread>
#include <atomic>
#include <functional>

namespace wnet {

	class Thread {
	public:
		Thread() :_stop(true){}
		virtual ~Thread() {}
	public:
		virtual void StartThread()
		{
			_stop = false;
			if (!_thread)
			{
				_thread = std::unique_ptr<std::thread>(new std::thread(std::bind(&Thread::Run,this)));
			}
		}

		virtual void StopThread()
		{
			_stop = true;
		}

		virtual void JoinThread()
		{
			if (_thread && _thread->joinable())
			{
				_thread->join();
			}
		}

		virtual void DetachThread()
		{
			if (_thread)
			{
				_thread->detach();
			}
		}

		virtual bool ThreadState() {
			return _stop;
		}

		virtual void Run() = 0;
	protected:
		Thread(const Thread&) = delete;
		Thread& operator=(const Thread&) = delete;
	protected:
		std::atomic_bool _stop;
		std::unique_ptr<std::thread>  _thread;
	};
}