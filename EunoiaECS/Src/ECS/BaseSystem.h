#pragma once

#include <vector>
#include "ECSTypeDefs.h"
#include "../Core/SubEngines.h"

namespace Eunoia { namespace ECS {

	class ECSEngine;

	typedef std::vector<ComponentTypeID> SystemSignature;

	class BaseSystem
	{
	public:
		BaseSystem();
		virtual ~BaseSystem();

		virtual void PreProcess(float dt) {}
		virtual void ProcessEntity(EntityID entityID, float dt) = 0;
		virtual void PostProcess(float dt) {}

		inline bool IsEnabled() const { return m_enabled; }
		inline void SetEnabled(bool enabled) { m_enabled = enabled; }

		inline SystemSignature& GetSignature() { return m_componentTypes; }

		inline static SystemTypeID RegisterSystemType() { return s_lastTypeID++; }
	protected:
		void AddComponentType(ComponentTypeID componentType);
		void RemoveComponentType(ComponentTypeID componentType);
	protected:
		friend class ECSEngine;
		ECSEngine* m_pECS;
		SubEngines m_subEngines;
		bool m_enabled;
	private:
		SystemSignature m_componentTypes;

		static SystemTypeID s_lastTypeID;
	};


} }
