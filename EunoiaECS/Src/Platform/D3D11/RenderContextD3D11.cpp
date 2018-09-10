#include "RenderContextD3D11.h"
#include "../Windows/Win32Display.h"

namespace Eunoia { namespace Rendering {

	RenderContextD3D11* RenderContextD3D11::s_pRenderContext = NULL;

	RenderContextD3D11::RenderContextD3D11() :
		m_vsync(true)
	{
		CreateDeviceAndSwapChain();
		CreateRenderTargetView();
		CreateDefaultRasterizerState();

		m_clearColor[0] = 0.0f;
		m_clearColor[1] = 0.0f;
		m_clearColor[2] = 0.0f;
		m_clearColor[3] = 1.0f;

		s_pRenderContext = this;
	}

	RenderContextD3D11::~RenderContextD3D11()
	{
		m_pSwapChain->Release();
		m_pDevice->Release();
		m_pDeviceContext->Release();
	}

	void RenderContextD3D11::SetVsync(bool vsync) const
	{
		m_vsync = vsync;
	}

	void RenderContextD3D11::SetClearColor(float r, float g, float b) const
	{
		m_clearColor[0] = r;
		m_clearColor[1] = g;
		m_clearColor[2] = b;
	}

	void RenderContextD3D11::Clear(ClearFlag clearFlag) const
	{
		if ((clearFlag & CLEAR_FLAG_COLOR_BUFFER) == CLEAR_FLAG_COLOR_BUFFER)
			m_pDeviceContext->ClearRenderTargetView(m_pRenderTargetView, m_clearColor);
		//if((clearFlag & CLEAR_FLAG_DEPTH_BUFFER) == CLEAR_FLAG_DEPTH_BUFFER)

		//if((clearFlag & CLEAR_FLAG_STENCIL_BUFFER) == CLEAR_FLAG_STENCIL_BUFFER)
	}

	void RenderContextD3D11::Swap() const
	{
		m_pSwapChain->Present(m_vsync ? 1 : 0, 0);
	}

	void RenderContextD3D11::SetViewport(float x, float y, float w, float h) const
	{
		D3D11_VIEWPORT viewport {};
		viewport.TopLeftX = x;
		viewport.TopLeftY = y;
		viewport.Height = h;
		viewport.Width = w;
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;

		m_pDeviceContext->RSSetViewports(1, &viewport);
	}

	void RenderContextD3D11::SetDisplayAsRenderTarget() const
	{
		m_pDeviceContext->OMSetRenderTargets(1, &m_pRenderTargetView, NULL);
	}

	IDXGISwapChain * RenderContextD3D11::GetSwapChain() const
	{
		return m_pSwapChain;
	}

	ID3D11Device * RenderContextD3D11::GetDevice() const
	{
		return m_pDevice;
	}

	ID3D11DeviceContext * RenderContextD3D11::GetDeviceContext() const
	{
		return m_pDeviceContext;
	}

	RenderContextD3D11 * RenderContextD3D11::GetRenderContext()
	{
		return s_pRenderContext;
	}

	void RenderContextD3D11::CreateDeviceAndSwapChain()
	{
		DXGI_SWAP_CHAIN_DESC swap_chain_desc {};
		swap_chain_desc.BufferCount = 1;
		swap_chain_desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		swap_chain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swap_chain_desc.OutputWindow = Win32Display::GetDisplay()->GetWindowHandle();
		swap_chain_desc.SampleDesc.Count = 1;
		swap_chain_desc.Windowed = true;

		HRESULT err = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, NULL, 0, D3D11_SDK_VERSION, &swap_chain_desc, &m_pSwapChain, &m_pDevice, NULL, &m_pDeviceContext);

		EU_ASSERT(err == S_OK, "Error creating d3d11 device and swapchain");
	}

	void RenderContextD3D11::CreateRenderTargetView()
	{
		ID3D11Texture2D* pBackBuffer;
		m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pBackBuffer);
		m_pDevice->CreateRenderTargetView(pBackBuffer, NULL, &m_pRenderTargetView);
		pBackBuffer->Release();
	}

	void RenderContextD3D11::CreateDefaultRasterizerState()
	{
		D3D11_RASTERIZER_DESC rasterizer_desc{};
		rasterizer_desc.CullMode = D3D11_CULL_NONE;
		rasterizer_desc.FillMode = D3D11_FILL_SOLID;
		rasterizer_desc.FrontCounterClockwise = true;
		rasterizer_desc.ScissorEnable = false;

		HRESULT err = m_pDevice->CreateRasterizerState(&rasterizer_desc, &m_pRasterizerState);
		EU_ASSERT(err == S_OK, "Error creating d3d11 rasterizer state");

		m_pDeviceContext->RSSetState(m_pRasterizerState);
	}


} }