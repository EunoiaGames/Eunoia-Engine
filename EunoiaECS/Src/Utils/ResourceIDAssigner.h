#pragma once

#include "../Core/Common.h"
#include <vector>

namespace Eunoia {

	class ResourceIDAssigner
	{
	public:
		uint32 GetNewID();
		void FreeID(uint32 id);
	private:
		uint32 m_lastID;
		std::vector<uint32> m_freeIDs;
	};

}
