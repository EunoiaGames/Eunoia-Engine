#pragma once

#include "Allocator.h"

namespace Eunoia {

	class PoolAllocator : public Allocator
	{
	public:
		PoolAllocator(uint32 size, uint32 numElements);
		PoolAllocator(void* pData, uint32 size, uint32 numElements);
		PoolAllocator();
		~PoolAllocator();

		void* Allocate(uint32 size = 0) override;
		void Free(void* pData) override;
		void Reset() override;

		void* GetData() const override;
	private:
		struct PoolElement
		{
			PoolElement* pNext;
		};

		void Init();
	private:
		uint32 m_numElements;
		uint32 m_elementSize;
		PoolElement* m_pElement;
		void* m_pData;
	};

}
