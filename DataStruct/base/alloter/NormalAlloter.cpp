#include "NormalAlloter.h"

namespace wnet
{
	NormalAlloter::NormalAlloter()
	{}

	NormalAlloter::~NormalAlloter()
	{}

	void* NormalAlloter::Alloc(uint32_t size)
	{
		void* ret = malloc(size);
		if (!ret)
		{
			throw "not enough memory";
			return nullptr;
		}
		return ret;
	}

	void* NormalAlloter::AllocZero(uint32_t size)
	{
		void* data = Alloc(size);
		if (data)
		{
			memset(data, 0, size);
		}
		return data;
	}

	void* NormalAlloter::AllocAlign(uint32_t size)
	{
		return Alloc(align(size));
	}

	void NormalAlloter::Free(void*& data, uint32_t len /*= 0*/)
	{
		free(data);
		data = nullptr;
	}

	std::shared_ptr<IAlloter> MakeNormalAlloterPrt()
	{
		return std::make_shared<NormalAlloter>();
	}
}

