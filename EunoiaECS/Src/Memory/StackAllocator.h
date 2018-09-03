#pragma once

#include "Allocator.h"

#define STACK_ALLOCATOR_OFFSET_SIZE sizeof(Eunoia::uint32)

namespace Eunoia {

	class StackAllocator : public Allocator
	{
	public:
		StackAllocator(uint32 size);
		StackAllocator(void* pData, uint32 size);
		~StackAllocator();

		void* Allocate(uint32 size) override;
		void Free(void* pData) override;
		void Reset() override;

		void* GetData() const override;
	private:
		void* m_pData;
		uint32 m_offset;
	};

}