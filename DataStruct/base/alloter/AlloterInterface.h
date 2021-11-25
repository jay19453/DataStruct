#pragma once
//接口类
#include <memory>
#include <cstdint>
#include "../../config/NetConfig.h"

namespace wnet{
	class IAlloter {
	public:
		IAlloter() {};
		virtual ~IAlloter() {};

		virtual void* Alloc(uint32_t size) = 0;
		virtual void* AllocZero(uint32_t size) = 0;
		virtual void* AllocAlign(uint32_t size) = 0;

		virtual void  Free(void*& data, uint32_t len = 0) = 0;
	protected:
		uint32_t align(uint32_t size)
		{
			return (size + __align - 1) & ~(__align - 1);
		}
	};

	class IAlloterWrap {
	public:
		IAlloterWrap(std::shared_ptr<IAlloter> a) :_alloter(a) {}
		~IAlloterWrap() {}
	public:
		template<typename T,typename... Args>
		T* PoolNew(Args&&... args);
		template<typename T,typename... Args>
		std::shared_ptr<T> PoolNewSharePtr(Args...args);

		template<typename T>
		void PoolDelete(T* &c);

		template<typename T>
		T* PoolMalloc(uint32_t size);
		template<typename T>
		std::shared_ptr<T> PoolMallocSharePtr(uint32_t size);

		template<typename T>
		void PoolFree(T* &m,uint32_t len);
	private:
		std::shared_ptr<IAlloter> _alloter;
	};

	template<typename T,typename... Args>
	T* IAlloterWrap::PoolNew(Args&&... args) {
		uint32_t size = sizeof(T);

		void* data = _alloter->AllocAlign(size);
		if (!data)
		{
			return nullptr;
		}
		//placement new
		//它并不分配内存，只是返回指向已经分配好的某段内存的一个指针。因此不能删除它，但需要调用对象的析构函数。
		T* res = new(data) T(std::forward<Args>(args)...);
		return res;
	}

	template<typename T, typename... Args>
	std::shared_ptr<T> IAlloterWrap::PoolNewSharePtr(Args...args)
	{
		T*	ret = PoolNew<T>(std::forward<Args>(args)...);
		return std::shared_ptr<T>(ret, [this](T* &c) {PoolDelete(c)});
	}

	template<typename T>
	void IAlloterWrap::PoolDelete(T* &c)
	{
		if (!c)
		{
			return;
		}
		c->~T();
		uint32_t len = sizeof(T);
		void* data = (void*)c;
		_alloter->Free(c, len);
		c = nullptr;
	}

	template<typename T>
	T* IAlloterWrap::PoolMalloc(uint32_t size)
	{
		return (T*)_alloter->AllocAlign(size);
	}

	template<typename T>
	std::shared_ptr<T> IAlloterWrap::PoolMallocSharePtr(uint32_t size)
	{
		T* ret = PoolMalloc(size);
		return std::shared_ptr<T>(ret, [this, size](T* &c) {PoolFree(c, size); });
	}

	template<typename T>
	void IAlloterWrap::PoolFree(T* &m, uint32_t len)
	{
		if (!m)
		{
			return;
		}
		void* data = (void*)m;
		_alloter->Free(data, len);
		m = nullptr;
	}
}


