#pragma once

#include "BaseSystem.h"
#include <map>

namespace Eunoia { namespace ECS {

	enum SystemPipeLineType
	{
		UPDATE_PIPELINE,
		RENDER_PIPELINE,

		NUM_SYSTEM_PIPELINE_TYPES
	};

	typedef std::map<SystemTypeID, BaseSystem*> SystemPipeline;

} }
