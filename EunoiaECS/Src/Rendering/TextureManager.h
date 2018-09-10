#pragma once

#ifdef EU_RENDER_API_OPENGL

#include "../Platform/Opengl/TextureManagerGL.h"
namespace Eunoia { namespace Rendering { typedef TextureManagerGL TextureManager; } }

#elif defined EU_RENDER_API_D3D11

#include "../Platform/D3D11/TextureManagerD3D11.h"
namespace Eunoia { namespace Rendering { typedef TextureManagerD3D11 TextureManager; } }

#elif defined EU_RENDER_API_VULKAN

#else
#define TextureManagerD3D11 void*
#endif