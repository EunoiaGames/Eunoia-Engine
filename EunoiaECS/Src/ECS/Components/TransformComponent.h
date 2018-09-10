#pragma once

#include "../Component.h"
#include "../../Math/Transform.h"

namespace Eunoia { namespace ECS {

	struct TransformComponent : public Component<TransformComponent>
	{
		TransformComponent(const Math::Transform& transform = Math::Transform()) :
			transform(transform)
		{}

		Math::Transform transform;
	};

} }
