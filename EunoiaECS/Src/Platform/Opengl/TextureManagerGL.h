#pragma once

#include "../../Rendering/RenderApiEnums.h"
#include <GL\glew.h>
#include "../../DataStructures/String.h"
#include <vector>
#include "../../Rendering/ResourceTypes.h"

namespace Eunoia { namespace Rendering {

	class TextureManagerGL : private ResourceIDAssigner
	{
	public:
		TextureManagerGL();
		~TextureManagerGL();

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
		struct TextureInfoGL
		{
			GLuint texture;
			GLenum target;
			uint32 width;
			uint32 height;
		};
	private:
		std::vector<TextureInfoGL> m_textures;
	};

} }
