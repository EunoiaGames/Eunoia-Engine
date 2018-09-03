#pragma once

#include "BaseSystem.h"

namespace Eunoia { namespace ECS {

	template<class S>
	class System : public BaseSystem
	{
	public:
		System() {}
		virtual ~System() {}

		static const SystemTypeID SYSTEM_TYPE_ID;
	};

	template<class S>
	const SystemTypeID System<S>::SYSTEM_TYPE_ID = BaseSystem::RegisterSystemType();
} }
