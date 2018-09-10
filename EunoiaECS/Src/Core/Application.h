#pragma once

#include "../ECS/ECSEngine.h"

namespace Eunoia { namespace Core {

	class Application
	{
	public:
		virtual void Init() = 0;
		virtual void ShutDown() {}
	protected:
		ECS::ECSEngine* m_pEcs;
	private:
		friend class CoreEngine;
	};

} }
