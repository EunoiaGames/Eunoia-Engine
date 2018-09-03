#pragma once

#include "../Core/Common.h"
#include "ECSTypeDefs.h"

namespace Eunoia { namespace ECS {

	class BaseComponent
	{
	public:
		inline BaseComponent() :
			m_enabled(true)
		{}

		inline virtual ~BaseComponent() {}

		inline void SetEnabled(bool enabled) { m_enabled = enabled; }
		inline bool IsEnabled() const { return m_enabled; }

		inline static ComponentTypeID RegisterComponentType() { return s_lastTypeID++; }
		inline static ComponentTypeID GetNumberOfTypeIDs() { return s_lastTypeID; }
	protected:
		bool m_enabled;
	private:
		static ComponentTypeID s_lastTypeID;
	};

} }
