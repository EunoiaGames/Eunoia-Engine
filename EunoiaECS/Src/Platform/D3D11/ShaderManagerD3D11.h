#pragma once

#include "../../Core/Common.h"
#include "../../DataStructures/String.h"
#include "../../Rendering/ShaderType.h"
#include "RenderContextD3D11.h"
#include <d3dcompiler.h>
#include <map>
#include "../../Rendering/ResourceTypes.h"

namespace Eunoia { namespace Rendering {

	class ShaderManagerD3D11 : private ResourceIDAssigner
	{
	public:
		ShaderManagerD3D11();
		~ShaderManagerD3D11();

		ShaderID CreateShader(const String& shaderFile);
		void DeleteShader(ShaderID shaderID);

		void BindShader(ShaderID shaderID) const;

		void BindTextureToShader(ShaderID shaderID, TextureID textureID, const String& nameOfTextureInShader) const;
	private:
		static ID3DBlob* CompileShader(const String& sourceName, const String& shaderSrc, const String& entryPoint, const String& target);
		struct ShaderInfoD3D11;
		static void ParseShader(ID3D11ShaderReflection* pReflection, ShaderInfoD3D11& shader, ShaderType type);
	private:
		struct ShaderInfoD3D11
		{
			ID3D11VertexShader* pVertexShader;
			ID3D11PixelShader* pPixelShader;
			std::map<String, TextureUnit> psTextureUnits;
		};
	private:
		RenderContextD3D11* m_pRenderContext;
		std::vector<ShaderInfoD3D11> m_shaders;
	};

} }
