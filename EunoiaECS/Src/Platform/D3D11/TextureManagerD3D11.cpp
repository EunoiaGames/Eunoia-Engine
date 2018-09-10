#include "TextureManagerD3D11.h"
#include "../../StaticLibs/stb_image.h"

namespace Eunoia { namespace Rendering {

	TextureManagerD3D11::TextureManagerD3D11()
	{
		m_pRenderContext = RenderContextD3D11::GetRenderContext();
	}

	TextureManagerD3D11::~TextureManagerD3D11()
	{
		for (uint32 i = 0; i < m_textures.size(); i++)
		{
			const TextureInfoD3D11& texture = m_textures[i];
			texture.pSamplerState->Release();
			texture.pShaderResourceView->Release();
		}
	}

	TextureID TextureManagerD3D11::CreateTexture2D(const unsigned char * pPixels, uint32 width, uint32 height, TextureFormat format, TextureFilter filter, TextureWrap wrap)
	{
		TextureInfoD3D11 texture;
		texture.width = width;
		texture.height = height;
		
		// create texture 2d

		D3D11_TEXTURE2D_DESC texture_desc {};
		texture_desc.Width = width;
		texture_desc.Height = height;
		texture_desc.MipLevels = texture_desc.ArraySize = 1;
		texture_desc.Format = (DXGI_FORMAT)format;
		texture_desc.SampleDesc.Count = 1;
		texture_desc.Usage = D3D11_USAGE_DYNAMIC;
		texture_desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		texture_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		texture_desc.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA initial_data {};
		initial_data.pSysMem = pPixels;
		initial_data.SysMemPitch = width * 4 * sizeof(unsigned char);
		initial_data.SysMemSlicePitch = 0;

		ID3D11Texture2D* pTexture2D;
		HRESULT err = m_pRenderContext->GetDevice()->CreateTexture2D(&texture_desc, &initial_data, &pTexture2D);
		EU_ASSERT(err == S_OK, "Error creating d3d11 texture2d");
		
		// create resource view

		D3D11_SHADER_RESOURCE_VIEW_DESC resource_view_desc {};
		resource_view_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		resource_view_desc.Format = (DXGI_FORMAT)format;
		resource_view_desc.Texture2D.MostDetailedMip = 0;
		resource_view_desc.Texture2D.MipLevels = 1;

		err = m_pRenderContext->GetDevice()->CreateShaderResourceView(pTexture2D, &resource_view_desc, &texture.pShaderResourceView);
		EU_ASSERT(err == S_OK, "Error creating d3d11 resource view");
		pTexture2D->Release();

		// create sampler state

		D3D11_SAMPLER_DESC sampler_desc {};
		sampler_desc.Filter = (D3D11_FILTER)filter;
		sampler_desc.AddressU = (D3D11_TEXTURE_ADDRESS_MODE)wrap;
		sampler_desc.AddressV = (D3D11_TEXTURE_ADDRESS_MODE)wrap;
		sampler_desc.AddressW = (D3D11_TEXTURE_ADDRESS_MODE)wrap;
		sampler_desc.MipLODBias = 0.0f;
		sampler_desc.MaxAnisotropy = 1;
		sampler_desc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
		sampler_desc.BorderColor[0] = 0;
		sampler_desc.BorderColor[1] = 0;
		sampler_desc.BorderColor[2] = 0;
		sampler_desc.BorderColor[3] = 0;
		sampler_desc.MinLOD = 0;
		sampler_desc.MaxLOD = D3D11_FLOAT32_MAX;

		err = m_pRenderContext->GetDevice()->CreateSamplerState(&sampler_desc, &texture.pSamplerState);
		EU_ASSERT(err == S_OK, "Error creating d3d11 sampler state");

		//return id

		TextureID id = GetNewID();
		if (id - 1 == m_textures.size())
			m_textures.push_back(texture);
		else
			m_textures[id - 1] = texture;
		return id;
	}

	TextureID TextureManagerD3D11::CreateTexture2D(const String & textureFile, TextureFormat format, TextureFilter filter, TextureWrap wrap)
	{
		int width, height;
		const unsigned char* pPixels = stbi_load(textureFile.C_Str(), &width, &height, NULL, 4);
		return CreateTexture2D(pPixels, width, height, format, filter, wrap);
	}

	void TextureManagerD3D11::DeleteTexture(TextureID textureID)
	{
		m_textures[textureID - 1].pShaderResourceView->Release();
		FreeID(textureID);
	}

	void TextureManagerD3D11::SetFilter(TextureID textureID, TextureFilter filter)
	{
		TextureInfoD3D11& texture = m_textures[textureID - 1];
		D3D11_SAMPLER_DESC sampler_desc;
		texture.pSamplerState->GetDesc(&sampler_desc);
		texture.pSamplerState->Release();
		sampler_desc.Filter = (D3D11_FILTER)filter;
		m_pRenderContext->GetDevice()->CreateSamplerState(&sampler_desc, &texture.pSamplerState);
	}

	void TextureManagerD3D11::SetWrap(TextureID textureID, TextureWrap wrap)
	{
		TextureInfoD3D11& texture = m_textures[textureID - 1];
		D3D11_SAMPLER_DESC sampler_desc;
		texture.pSamplerState->GetDesc(&sampler_desc);
		texture.pSamplerState->Release();
		sampler_desc.AddressU = (D3D11_TEXTURE_ADDRESS_MODE)wrap;
		sampler_desc.AddressV = (D3D11_TEXTURE_ADDRESS_MODE)wrap;
		sampler_desc.AddressW = (D3D11_TEXTURE_ADDRESS_MODE)wrap;
		m_pRenderContext->GetDevice()->CreateSamplerState(&sampler_desc, &texture.pSamplerState);
	}

	void TextureManagerD3D11::SetTextureParams(TextureID textureID, TextureFilter filter, TextureWrap wrap)
	{
		TextureInfoD3D11& texture = m_textures[textureID - 1];
		D3D11_SAMPLER_DESC sampler_desc;
		texture.pSamplerState->GetDesc(&sampler_desc);
		texture.pSamplerState->Release();
		sampler_desc.Filter = (D3D11_FILTER)filter;
		sampler_desc.AddressU = (D3D11_TEXTURE_ADDRESS_MODE)wrap;
		sampler_desc.AddressV = (D3D11_TEXTURE_ADDRESS_MODE)wrap;
		sampler_desc.AddressW = (D3D11_TEXTURE_ADDRESS_MODE)wrap;
		m_pRenderContext->GetDevice()->CreateSamplerState(&sampler_desc, &texture.pSamplerState);
	}

	void TextureManagerD3D11::BindTexture(TextureID textureID, TextureUnit unit) const
	{
		const TextureInfoD3D11& texture = m_textures[textureID - 1];
		m_pRenderContext->GetDeviceContext()->PSSetSamplers(0, 1, &texture.pSamplerState);
		m_pRenderContext->GetDeviceContext()->PSSetShaderResources(0, 1, &texture.pShaderResourceView);
	}

} }
