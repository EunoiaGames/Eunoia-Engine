#include "BaseSystem.h"

namespace Eunoia { namespace ECS {

	SystemTypeID BaseSystem::s_lastTypeID = 0;

	BaseSystem::BaseSystem()
	{
	}

	BaseSystem::~BaseSystem()
	{
	}

	void BaseSystem::AddComponentType(ComponentTypeID componentType)
	{
		m_componentTypes.push_back(componentType);
	}

	void BaseSystem::RemoveComponentType(ComponentTypeID componentType)
	{
		for (uint32 i = 0; i < m_componentTypes.size(); i++)
		{
			if (m_componentTypes[i] == componentType)
			{
				m_componentTypes.erase(m_componentTypes.begin() + i);
				break;
			}
		}
	}

} }
