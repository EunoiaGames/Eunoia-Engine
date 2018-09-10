#pragma once

#include "../Windows/Win32Display.h"
#include "../../Rendering/RenderApiEnums.h"
#include <d3d11.h>

namespace Eunoia { namespace Rendering {

	class RenderContextD3D11
	{
	public:
		RenderContextD3D11();
		~RenderContextD3D11();

		void SetVsync(bool vsync) const;

		void SetClearColor(float r, float g, float b) const;
		void Clear(ClearFlag clearFlag) const;
		void Swap() const;
		void SetViewport(float x, float y, float w, float h) const;
		void SetDisplayAsRenderTarget() const;

		IDXGISwapChain* GetSwapChain() const;
		ID3D11Device* GetDevice() const;
		ID3D11DeviceContext* GetDeviceContext() const;

		static RenderContextD3D11* GetRenderContext();
	private:
		void CreateDeviceAndSwapChain();
		void CreateRenderTargetView();
		void CreateDefaultRasterizerState();
	private:
		IDXGISwapChain* m_pSwapChain;
		ID3D11Device* m_pDevice;
		ID3D11DeviceContext* m_pDeviceContext;

		ID3D11RenderTargetView* m_pRenderTargetView;
		ID3D11RasterizerState* m_pRasterizerState;

		mutable bool m_vsync;
		mutable float m_clearColor[4];

		static RenderContextD3D11* s_pRenderContext;
	};

} }
