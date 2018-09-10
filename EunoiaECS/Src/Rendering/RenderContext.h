#pragma once

#include "../Core/Common.h"

#ifdef EU_RENDER_API_OPENGL

#include "../Platform/Opengl/RenderContextGL.h"
namespace Eunoia { namespace Rendering { typedef RenderContextGL RenderContext; } }

#elif defined EU_RENDER_API_D3D11

#include "../Platform/D3D11/RenderContextD3D11.h"
namespace Eunoia { namespace Rendering { typedef RenderContextD3D11 RenderContext; } }

#elif defined EU_RENDER_API_VULKAN



#else



#endif
