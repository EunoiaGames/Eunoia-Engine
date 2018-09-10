#pragma once

#include "../Core/Common.h"
#include "../Utils/ResourceIDAssigner.h"

namespace Eunoia { namespace Rendering {

#define EU_INVALID_TEXTURE_UNIT 100
#define EU_INVALID_MESH_ID 0
#define EU_INVALID_SHADER_ID 0
#define EU_INVALID_TEXTURE_ID 0
#define EU_INVALID_MATERIAL_ID 0

	typedef uint32 MeshID;
	typedef uint32 ShaderID;
	typedef uint32 TextureID;
	typedef uint32 TextureUnit;
	typedef uint32 MaterialID;
} }