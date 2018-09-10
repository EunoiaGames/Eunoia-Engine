#pragma once

#include "../System.h"

namespace Eunoia { namespace ECS {

	class RenderableSubmissionSystem : public System<RenderableSubmissionSystem>
	{
	public:
		RenderableSubmissionSystem();
		void ProcessEntity(EntityID entityID, float dt) override;
	private:

	};

} }
