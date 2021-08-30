#pragma once
//�ӿ���
#include <memory>
#include <cstdint>

namespace base{
	static const uint16_t __align = sizeof(unsigned long);
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
}

