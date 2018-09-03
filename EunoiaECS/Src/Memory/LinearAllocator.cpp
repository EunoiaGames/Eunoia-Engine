#include "LinearAllocator.h"
#include <cstdlib>

namespace Eunoia {

	LinearAllocator::LinearAllocator(uint32 size)
	{
		m_pData = malloc(size);
		m_offset = 0;
		m_size = size;
	}

	LinearAllocator::LinearAllocator(void * pData, uint32 size)
	{
		m_pData = pData;
		m_offset = 0;
		m_size = size;
	}

	LinearAllocator::~LinearAllocator()
	{
		
	}

	void * LinearAllocator::Allocate(uint32 size)
	{
		if (m_offset + size > m_size)
			return NULL;
		void* pNewMem = (unsigned char*)m_pData + m_offset;
		m_offset += size;
		return pNewMem;
	}

	void LinearAllocator::Free(void * pData)
	{
		//Can't free memory with this allocator
	}

	void LinearAllocator::Reset()
	{
		m_offset = 0;
	}

	void * LinearAllocator::GetData() const
	{
		return m_pData;
	}

}

