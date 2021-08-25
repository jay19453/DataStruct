#pragma once
#include "Alloc.h"

template <class T>
class allocator {
public:
	typedef T value_type;
	typedef T* pointer;
	typedef const T* const_pointer;
	typedef T&  reference;
	typedef const T& const_reference;
	typedef size_t size_type;
	typedef ptrdiff_t difference_type;
public:
	static pointer allocate();
	static pointer allocate(size_type n);
	static void deallocate(pointer ptr);
	static void deallocate(pointer ptr,size_type n);

	static void construct(pointer ptr);
	static void construct(pointer ptr,const_reference value);
	static void destroy(pointer ptr);
	static void destroy(pointer first,pointer last);
	static void destroyAlloc();
//private:
//	class alloc {
//
//	public:
//		static void* allocate(size_type n);
//	};
};

template<class T>
pointer allocator<T>::allocate()
{
	return static_cast<T*>(alloc::allocate(sizeof(T)));
}

template<class T>
pointer allocator<T>::allocate(size_type n)
{
	return static_cast<T*>(alloc::allocate(sizeof(T) * n));
}

template<class T>
void allocator<T>::deallocate(pointer ptr)
{
	alloc::dellocate(static_cast<void*>(ptr),sizeof(T));
}

template<class T>
void allocator<T>::deallocate(pointer ptr, size_type n)
{
	if (0 == n) return;
	alloc::dellocate(static_cast<void*>(ptr),sizeof(T)*n);
}

template<class T>
void allocator<T>::construct(pointer ptr)
{
	new(ptr)T();
}

template<class T>
void allocator<T>::construct(pointer ptr, const_reference value)
{
	new(ptr)T(value);
}

template<class T>
void allocator<T>::destroy(pointer ptr)
{
	ptr->~T();
}

template<class T>
void allocator<T>::destroy(pointer first, pointer last)
{
	for (;first != last;++first)
	{
		first->~T();
	}
}

template<class T>
void allocator<T>::destroyAlloc()
{
	alloc::release();
}