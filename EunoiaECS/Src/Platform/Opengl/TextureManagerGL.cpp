#include "TextureManagerGL.h"
#include "../../StaticLibs/stb_image.h"

namespace Eunoia { namespace Rendering {

	TextureManagerGL::TextureManagerGL()
	{
	}

	TextureManagerGL::~TextureManagerGL()
	{
		for (uint32 i = 0; i < m_textures.size(); i++)
			glDeleteTextures(1, &m_textures[i].texture);
	}

	TextureID TextureManagerGL::CreateTexture2D(const unsigned char * pPixels, uint32 width, uint32 height, TextureFormat format, TextureFilter filter, TextureWrap wrap)
	{
		TextureInfoGL texture;
		texture.target = GL_TEXTURE_2D;
		texture.width = width;
		texture.height = height;
		glGenTextures(1, &texture.texture);
		glBindTexture(GL_TEXTURE_2D, texture.texture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pPixels);
		glBindTexture(GL_TEXTURE_2D, 0);
		
		TextureID id = GetNewID();
		if (id - 1 == m_textures.size())
			m_textures.push_back(texture);
		else
			m_textures[id - 1] = texture;
		return id;
	}

	TextureID TextureManagerGL::CreateTexture2D(const String & textureFile, TextureFormat format, TextureFilter filter, TextureWrap wrap)
	{
		int32 width, height;
		const unsigned char* pPixels = stbi_load(textureFile.C_Str(), &width, &height, NULL, 4);
		return CreateTexture2D(pPixels, width, height, format, filter, wrap);
	}

	void TextureManagerGL::DeleteTexture(TextureID textureID)
	{
		const TextureInfoGL& texture = m_textures[textureID - 1];
		glDeleteTextures(1, &texture.texture);
		FreeID(textureID);
	}

	void TextureManagerGL::SetFilter(TextureID textureID, TextureFilter filter)
	{
		const TextureInfoGL& texture = m_textures[textureID - 1];
		glBindTexture(texture.target, texture.texture);
		glTexParameteri(texture.target, GL_TEXTURE_MIN_FILTER, filter);
		glTexParameteri(texture.target, GL_TEXTURE_MAG_FILTER, filter);
		glBindTexture(texture.target, 0);
	}

	void TextureManagerGL::SetWrap(TextureID textureID, TextureWrap wrap)
	{
		const TextureInfoGL& texture = m_textures[textureID - 1];
		glBindTexture(texture.target, texture.texture);
		glTexParameteri(texture.target, GL_TEXTURE_WRAP_S, wrap);
		glTexParameteri(texture.target, GL_TEXTURE_WRAP_T, wrap);
		glBindTexture(texture.target, 0);
	}

	void TextureManagerGL::SetTextureParams(TextureID textureID, TextureFilter filter, TextureWrap wrap)
	{
		const TextureInfoGL& texture = m_textures[textureID - 1];
		glBindTexture(texture.target, texture.texture);
		glTexParameteri(texture.target, GL_TEXTURE_MIN_FILTER, filter);
		glTexParameteri(texture.target, GL_TEXTURE_MAG_FILTER, filter);
		glTexParameteri(texture.target, GL_TEXTURE_WRAP_S, wrap);
		glTexParameteri(texture.target, GL_TEXTURE_WRAP_T, wrap);
		glBindTexture(texture.target, 0);
	}

	void TextureManagerGL::BindTexture(TextureID textureID, TextureUnit unit) const
	{
		glActiveTexture(GL_TEXTURE0 + unit);
		glBindTexture(m_textures[textureID - 1].target, m_textures[textureID - 1].texture);
	}

} }
