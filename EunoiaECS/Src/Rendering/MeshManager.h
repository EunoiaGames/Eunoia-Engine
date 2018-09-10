#pragma once


#ifdef EU_RENDER_API_OPENGL

#include "../Platform/Opengl/MeshManagerGL.h"
namespace Eunoia { namespace Rendering { typedef MeshManagerGL MeshManager; } }

#elif defined EU_RENDER_API_D3D11

#include "../Platform/D3D11/MeshManagerD3D11.h"
namespace Eunoia { namespace Rendering { typedef MeshManagerD3D11 MeshManager; } }

#elif defined EU_RENDER_API_VULKAN

#else
#define MeshManager void*
#endif