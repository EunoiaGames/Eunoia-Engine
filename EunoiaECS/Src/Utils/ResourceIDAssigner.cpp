#include "ResourceIDAssigner.h"

namespace Eunoia {

	uint32 ResourceIDAssigner::GetNewID()
	{
		if (!m_freeIDs.empty())
		{
			uint32 id = m_freeIDs.back();
			m_freeIDs.pop_back();
			return id;
		}
		else
		{
			uint32 id = ++m_lastID;
			return id;
		}
	}

	void ResourceIDAssigner::FreeID(uint32 id)
	{
		m_freeIDs.push_back(id);
	}
}
