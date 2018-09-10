#pragma once

#ifdef EU_RENDER_API_OPENGL

#include "../Platform/Opengl/ShaderManagerGL.h"
namespace Eunoia { namespace Rendering { typedef ShaderManagerGL ShaderManager; } }

#elif defined EU_RENDER_API_D3D11

#include "../Platform/D3D11/ShaderManagerD3D11.h"
namespace Eunoia { namespace Rendering { typedef ShaderManagerD3D11 ShaderManager; } }

#elif defined EU_RENDER_API_VULKAN

#else
#define ShaderManager void*
#endif
