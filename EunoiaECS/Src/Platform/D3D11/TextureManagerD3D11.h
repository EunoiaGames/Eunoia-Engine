#pragma once

#include "../../Rendering/RenderApiEnums.h"
#include "../../Core/Common.h"
#include "../../DataStructures/String.h"
#include "RenderContextD3D11.h"
#include "../../Rendering/ResourceTypes.h"

namespace Eunoia { namespace Rendering {

	class TextureManagerD3D11 : private ResourceIDAssigner
	{
	public:
		TextureManagerD3D11();
		~TextureManagerD3D11();

		TextureID CreateTexture2D(const unsigned char* pPixels, uint32 width, uint32 height,
			TextureFormat format = TEXTURE_FORMAT_RGBA8_UNORM, TextureFilter filter = TEXTURE_FILTER_LINEAR,
			TextureWrap wrap = TEXTURE_WRAP_REPEAT);

		TextureID CreateTexture2D(const String& textureFile, TextureFormat format = TEXTURE_FORMAT_RGBA8_UNORM,
			TextureFilter filter = TEXTURE_FILTER_LINEAR, TextureWrap wrap = TEXTURE_WRAP_REPEAT);

		void DeleteTexture(TextureID textureID);

		void SetFilter(TextureID textureID, TextureFilter filter);
		void SetWrap(TextureID textureID, TextureWrap wrap);
		void SetTextureParams(TextureID textureID, TextureFilter filter, TextureWrap wrap);

		void BindTexture(TextureID textureID, TextureUnit unit) const;
	private:
		struct TextureInfoD3D11
		{
			ID3D11ShaderResourceView* pShaderResourceView;
			ID3D11SamplerState* pSamplerState;
			uint32 width;
			uint32 height;
		};
	private:
		RenderContextD3D11* m_pRenderContext;

		std::vector<TextureInfoD3D11> m_textures;
	};

} }
