#include "ShaderManagerD3D11.h"
#include "../../Utils/FileReader.h"
#include "../../Rendering/ResourceManager.h"

namespace Eunoia { namespace Rendering {

	ShaderManagerD3D11::ShaderManagerD3D11()
	{
		m_pRenderContext = RenderContextD3D11::GetRenderContext();
	}

	ShaderManagerD3D11::~ShaderManagerD3D11()
	{
		for (uint32 i = 0; i < m_shaders.size(); i++)
		{
			const ShaderInfoD3D11& shader = m_shaders[i];
			shader.pVertexShader->Release();
			shader.pPixelShader->Release();
		}
	}

	ShaderID ShaderManagerD3D11::CreateShader(const String & shaderFile)
	{
		ShaderInfoD3D11 shader;

		String shaderSrc = FileReader::ReadFile(String("Res/Shaders/HLSL/") + shaderFile + ".hlsl");

		ID3DBlob* pVertexCode = CompileShader(shaderFile, shaderSrc, "VSMain", "vs_5_0");
		ID3DBlob* pPixelCode = CompileShader(shaderFile, shaderSrc, "PSMain", "ps_5_0");

		HRESULT err = m_pRenderContext->GetDevice()->CreateVertexShader(pVertexCode->GetBufferPointer(), pVertexCode->GetBufferSize(), NULL, &shader.pVertexShader);
		EU_ASSERT(err == S_OK, "Error creating d3d11 vertex shader");

		err = m_pRenderContext->GetDevice()->CreatePixelShader(pPixelCode->GetBufferPointer(), pPixelCode->GetBufferSize(), NULL, &shader.pPixelShader);
		EU_ASSERT(err == S_OK, "Error creating d3d11 pixel shader");

		ID3D11ShaderReflection* pVertexShaderReflection;
		D3DReflect(pVertexCode->GetBufferPointer(), pVertexCode->GetBufferSize(), IID_ID3D11ShaderReflection, (void**)&pVertexShaderReflection);
		ParseShader(pVertexShaderReflection, shader, SHADER_TYPE_VERTEX);

		ID3D11ShaderReflection* pPixelShaderReflection;
		D3DReflect(pPixelCode->GetBufferPointer(), pPixelCode->GetBufferSize(), IID_ID3D11ShaderReflection, (void**)&pPixelShaderReflection);
		ParseShader(pPixelShaderReflection, shader, SHADER_TYPE_PIXEL);

		pVertexCode->Release();
		pPixelCode->Release();

		ShaderID id = GetNewID();
		if (id - 1 == m_shaders.size())
			m_shaders.push_back(shader);
		else
			m_shaders[id - 1] = shader;
		return id;
	}

	void ShaderManagerD3D11::DeleteShader(ShaderID shaderID)
	{
		const ShaderInfoD3D11& shader = m_shaders[shaderID - 1];
		shader.pVertexShader->Release();
		shader.pPixelShader->Release();
		FreeID(shaderID);
	}

	void ShaderManagerD3D11::BindShader(ShaderID shaderID) const
	{
		const ShaderInfoD3D11& shader = m_shaders[shaderID - 1];
		m_pRenderContext->GetDeviceContext()->VSSetShader(shader.pVertexShader, NULL, 0);
		m_pRenderContext->GetDeviceContext()->PSSetShader(shader.pPixelShader, NULL, 0);
	}

	void ShaderManagerD3D11::BindTextureToShader(ShaderID shaderID, TextureID textureID, const String & nameOfTextureInShader) const
	{
		const ShaderInfoD3D11& shader = m_shaders[shaderID - 1];
		const auto&& it = shader.psTextureUnits.find(nameOfTextureInShader);
		if (it == shader.psTextureUnits.end())
			return;
		ResourceManager::pTextureManager->BindTexture(textureID, it->second);
	}

	ID3DBlob * ShaderManagerD3D11::CompileShader(const String& sourceName, const String & shaderSrc, const String& entryPoint, const String& target)
	{
		ID3DBlob* pCode;
		ID3DBlob* pErrMsgs;

		D3DCompile(shaderSrc.C_Str(), shaderSrc.Length(), sourceName.C_Str(), NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE, entryPoint.C_Str(),
			target.C_Str(), D3DCOMPILE_DEBUG, 0, &pCode, &pErrMsgs);

		if (pErrMsgs)
		{
			std::cerr << "Error creating d3d11 shader :" << std::endl << ((char*)pErrMsgs->GetBufferPointer()) << std::endl;
			pErrMsgs->Release();
			return NULL;
		}

		return pCode;
	}

	void ShaderManagerD3D11::ParseShader(ID3D11ShaderReflection * pReflection, ShaderInfoD3D11& shader, ShaderType type)
	{
		D3D11_SHADER_DESC shader_desc {};
		pReflection->GetDesc(&shader_desc);

		for (uint32 i = 0; i < shader_desc.BoundResources; i++)
		{
			D3D11_SHADER_INPUT_BIND_DESC bind_desc{};
			pReflection->GetResourceBindingDesc(i, &bind_desc);

			if (bind_desc.Type == D3D_SIT_CBUFFER)
			{

			}
			else if (bind_desc.Type == D3D_SIT_TEXTURE)
			{
				if (bind_desc.Dimension == D3D_SRV_DIMENSION_TEXTURE2D && type == SHADER_TYPE_PIXEL)
				{
					shader.psTextureUnits[String(bind_desc.Name)] = bind_desc.BindPoint;
				}
			}
		}
	}

} }
