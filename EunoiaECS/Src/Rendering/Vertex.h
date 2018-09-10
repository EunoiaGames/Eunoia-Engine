#pragma once
#include "../Math/Math.h"

namespace Eunoia { namespace Rendering {

	struct Vertex
	{
		Vertex(const Math::Vector3f& pos, const Math::Vector2f& texCoord) :
			pos(pos),
			texCoord(texCoord)
		{}

		Math::Vector3f pos;
		Math::Vector2f texCoord;
	};

} }
