#pragma once

namespace wnet {
	template <typename T>
	class Singleton
	{
	public:
		//magic static 如果当变量在初始化的时候，并发同时进入声明语句，并发线程将会阻塞等待初始化结束。
		static T& get_instance() {
			static T instance;
			return instance;
		}

		Singleton(const Singleton&) = delete;
		Singleton& operator=(const Singleton&) = delete;
	protected:
		Singleton() {}
		virtual ~Singleton() {}
	};
}
