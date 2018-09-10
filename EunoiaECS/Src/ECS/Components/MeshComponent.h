#pragma once

#include "../Component.h"
#include "../../Rendering/ResourceTypes.h"

namespace Eunoia { namespace ECS {

	struct MeshComponent : public Component<MeshComponent>
	{
		MeshComponent(Rendering::MeshID meshID, Rendering::MaterialID materialID) :
			meshID(meshID),
			materialID(materialID)
		{}

		Rendering::MeshID meshID;
		Rendering::MaterialID materialID;
	};

} }
