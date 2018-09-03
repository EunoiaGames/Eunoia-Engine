#include "PoolAllocator.h"
#include <cstdlib>
#include <iostream>

namespace Eunoia {

	PoolAllocator::PoolAllocator(uint32 size, uint32 numElements) 
	{
		m_pElement = (PoolElement*)malloc(size);
		m_pData = m_pElement;
		m_numElements = numElements;
		m_elementSize = size / numElements;
		Init();
	}

	PoolAllocator::PoolAllocator(void * pData, uint32 size, uint32 numElements)
	{
		m_pElement = (PoolElement*)pData;
		m_pData = m_pElement;
		m_numElements = numElements;
		m_elementSize = size / numElements;
		Init();
	}

	PoolAllocator::PoolAllocator() :
		m_numElements(0),
		m_elementSize(0),
		m_pElement(NULL)
	{}

	PoolAllocator::~PoolAllocator()
	{
		
	}

	void * PoolAllocator::Allocate(uint32 size)
	{
		if (!m_pElement)
			return NULL;

		PoolElement* pHead = m_pElement;
		m_pElement = m_pElement->pNext;
		return pHead;
	}

	void PoolAllocator::Free(void * pData)
	{
		PoolElement* head = (PoolElement*)(pData);
		head->pNext = m_pElement;
		m_pElement = head;
	}

	void PoolAllocator::Reset()
	{

	}

	void * PoolAllocator::GetData() const
	{
		return m_pData;
	}

	void PoolAllocator::Init()
	{
		union
		{
			PoolElement* as_self;
			unsigned char* as_char;
			void* as_void;
		};

		as_self = m_pElement;

		PoolElement* pRunner = m_pElement;
		for (uint32 i = 0; i < m_numElements; i++)
		{
			pRunner->pNext = as_self;
			pRunner = as_self;
			as_char += m_elementSize;
		}

		pRunner->pNext = NULL;
	}
}
