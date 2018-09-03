#include "StackAllocator.h"
#include <cstdlib>
#include <cstring>

namespace Eunoia {

	StackAllocator::StackAllocator(uint32 size)
	{
		m_pData = malloc(size);
		m_offset = 0;
	}

	StackAllocator::StackAllocator(void * pData, uint32 size)
	{
		m_pData = pData;
		m_offset = 0;
	}

	StackAllocator::~StackAllocator()
	{
		
	}

	void * StackAllocator::Allocate(uint32 size)
	{
		unsigned char* pNewMem = (unsigned char*)m_pData + m_offset;
		*(uint32*)pNewMem = m_offset;
		pNewMem += STACK_ALLOCATOR_OFFSET_SIZE;
		m_offset += size + STACK_ALLOCATOR_OFFSET_SIZE;
		return pNewMem;
	}

	void StackAllocator::Free(void * pData)
	{
		m_offset = *(uint32*)((unsigned char*)pData - STACK_ALLOCATOR_OFFSET_SIZE);
	}

	void StackAllocator::Reset()
	{
		m_offset = 0;
	}

	void * StackAllocator::GetData() const
	{
		return m_pData;
	}

}
