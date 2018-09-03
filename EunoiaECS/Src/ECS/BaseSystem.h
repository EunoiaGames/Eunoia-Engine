#pragma once

#include <vector>
#include "ECSTypeDefs.h"

namespace Eunoia { namespace ECS {

	class ECSEngine;

	class BaseSystem
	{
	public:
		BaseSystem();
		virtual ~BaseSystem();

		virtual void PreUpdate(float dt) {}
		virtual void UpdateEntity(EntityID entityID, float dt) = 0;
		virtual void PostUpdate(float dt) {}

		inline bool IsEnabled() const { return m_enabled; }
		inline void SetEnabled(bool enabled) { m_enabled = enabled; }

		inline std::vector<ComponentTypeID>& GetComponentTypes() { return m_componentTypes; }

		inline static SystemTypeID RegisterSystemType() { return s_lastTypeID++; }
	protected:
		void AddComponentType(ComponentTypeID componentType);
		void RemoveComponentType(ComponentTypeID componentType);
	protected:
		friend class ECSEngine;
		ECSEngine* m_pECS;
		bool m_enabled;
	private:
		std::vector<ComponentTypeID> m_componentTypes;

		static SystemTypeID s_lastTypeID;
	};


} }
