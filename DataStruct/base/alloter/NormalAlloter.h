#pragma once
#include "AlloterInterface.h"

namespace wnet {
	class NormalAlloter :public IAlloter {
	public:
		NormalAlloter();
		virtual ~NormalAlloter();
	public:
		void* Alloc(uint32_t size);
		void* AllocZero(uint32_t size);
		void* AllocAlign(uint32_t size);

		void  Free(void*& data, uint32_t len = 0);
	};
	std::shared_ptr<IAlloter> MakeNormalAlloterPrt();
}
