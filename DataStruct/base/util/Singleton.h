#pragma once

namespace wnet {
	template <typename T>
	class Singleton
	{
	public:
		//magic static ����������ڳ�ʼ����ʱ�򣬲���ͬʱ����������䣬�����߳̽��������ȴ���ʼ��������
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
