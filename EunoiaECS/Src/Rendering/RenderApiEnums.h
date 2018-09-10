#pragma once

#include "../Core/Common.h"

#ifdef EU_RENDER_API_D3D11

#include <d3d11.h>

namespace Eunoia {

#define CLEAR_FLAG_COLOR_BUFFER 0
#define CLEAR_FLAG_DEPTH_BUFFER 1
#define CLEAR_FLAG_STENCIL_BUFFER 2
	typedef uint32 ClearFlag;

	enum MeshUsage
	{
		MESH_USAGE_STATIC = D3D11_USAGE_DEFAULT,
		MESH_USAGE_DYNAMIC = D3D11_USAGE_DYNAMIC
	};

	enum TextureFilter
	{
		TEXTURE_FILTER_LINEAR = D3D11_FILTER_MIN_MAG_MIP_LINEAR,
		TEXTURE_FILTER_NEAREST = D3D11_FILTER_MIN_MAG_MIP_POINT
	};

	enum TextureWrap
	{
		TEXTURE_WRAP_REPEAT = D3D11_TEXTURE_ADDRESS_WRAP,
		TEXTURE_WRAP_CLAMP = D3D11_TEXTURE_ADDRESS_CLAMP,
	};

	enum TextureFormat
	{
		TEXTURE_FORMAT_RGBA8_UNORM = DXGI_FORMAT_R8G8B8A8_UNORM,
		TEXTURE_FORMAT_RGBA16_UNORM = DXGI_FORMAT_R16G16B16A16_UNORM,
		TEXTURE_FORMAT_RGB32_FLOAT = DXGI_FORMAT_R32G32B32_FLOAT,
		TEXTURE_FORMAT_RGBA16_FLOAT = DXGI_FORMAT_R16G16B16A16_FLOAT,
		TEXTURE_FORMAT_RGBA32_FLOAT = DXGI_FORMAT_R32G32B32A32_FLOAT,
		TEXTURE_FORMAT_RG16_FLOAT = DXGI_FORMAT_R16G16_FLOAT,
		TEXTURE_FORMAT_RG16_UNORM = DXGI_FORMAT_R16G16_UNORM,
		TEXTURE_FORMAT_RG32_FLOAT = DXGI_FORMAT_R32G32_FLOAT,

		TEXTURE_FORMAT_DEPTH16_UNORM = DXGI_FORMAT_D16_UNORM,
		TEXTURE_FORMAT_DEPTH24_UNORM = DXGI_FORMAT_D24_UNORM_S8_UINT,
		TEXTURE_FORMAT_DEPTH32_FLOAT = DXGI_FORMAT_D32_FLOAT,

		TEXTURE_FORMAT_DEPTH24_UNORM_STENCIL8 = DXGI_FORMAT_D24_UNORM_S8_UINT,
		TEXTURE_FORMAT_DEPTH32_FLOAT_STENCIL8 = DXGI_FORMAT_D32_FLOAT_S8X24_UINT,
	};
}

#elif defined EU_RENDER_API_OPENGL

#include <GL\glew.h>

namespace Eunoia {

#define CLEAR_FLAG_COLOR_BUFFER GL_COLOR_BUFFER_BIT
#define CLEAR_FLAG_DEPTH_BUFFER GL_DEPTH_BUFFER_BIT
#define CLEAR_FLAG_STENCIL_BUFFER GL_STENCIL_BUFFER_BIT
	typedef uint32 ClearFlag;

	enum MeshUsage
	{
		MESH_USAGE_STATIC = GL_STATIC_DRAW,
		MESH_USAGE_DYNAMIC = GL_DYNAMIC_DRAW
	};

	enum TextureFilter
	{
		TEXTURE_FILTER_LINEAR = GL_LINEAR,
		TEXTURE_FILTER_NEAREST = GL_NEAREST
	};

	enum TextureWrap
	{
		TEXTURE_WRAP_REPEAT = GL_REPEAT,
		TEXTURE_WRAP_CLAMP = GL_CLAMP,
	};

	enum TextureFormat
	{
		TEXTURE_FORMAT_RGBA8_UNORM = GL_RGBA8,
		TEXTURE_FORMAT_RGBA16_UNORM = GL_RGBA16,
		TEXTURE_FORMAT_RGB32_FLOAT = GL_RGB32F,
		TEXTURE_FORMAT_RGBA16_FLOAT = GL_RGBA16F,
		TEXTURE_FORMAT_RGBA32_FLOAT = GL_RGBA32F,
		TEXTURE_FORMAT_RG16_FLOAT = GL_RG16F,
		TEXTURE_FORMAT_RG16_UNORM = GL_RG16,
		TEXTURE_FORMAT_RG32_FLOAT = GL_RG32F,

		TEXTURE_FORMAT_DEPTH16_UNORM = GL_DEPTH_COMPONENT16,
		TEXTURE_FORMAT_DEPTH24_UNORM = GL_DEPTH_COMPONENT24,
		TEXTURE_FORMAT_DEPTH32_FLOAT = GL_DEPTH_COMPONENT32F,

		TEXTURE_FORMAT_DEPTH24_UNORM_STENCIL8 = GL_DEPTH24_STENCIL8,
		TEXTURE_FORMAT_DEPTH32_FLOAT_STENCIL8 = GL_DEPTH32F_STENCIL8,
	};
}

#elif defined EU_RENDER_API_VULKAN

#elif

#endif
