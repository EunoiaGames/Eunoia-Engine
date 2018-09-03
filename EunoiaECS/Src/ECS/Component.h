#pragma once

#include "BaseComponent.h"

namespace Eunoia { namespace ECS {

	template<typename C, uint32 MaxInstances = 512>
	class Component : public BaseComponent
	{
	public:
		Component() {}
		virtual ~Component() {}

		static const ComponentTypeID COMPONENT_TYPE_ID;
		static const uint32 MAX_INSTANCES;
	};

	template<typename C, uint32 MaxInstances>
	const ComponentTypeID Component<C, MaxInstances>::COMPONENT_TYPE_ID = BaseComponent::RegisterComponentType();

	template<typename C, uint32 MaxInstances>
	const uint32 Component<C, MaxInstances>::MAX_INSTANCES = MaxInstances;

} }
