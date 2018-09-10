#pragma once
#include <GL\glew.h>
#include <GL\wglew.h>
#include "../Windows/Win32Display.h"

#include "../../Rendering/RenderApiEnums.h"

namespace Eunoia { namespace Rendering {

	class RenderContextGL
	{
	public:
		RenderContextGL();
		~RenderContextGL();

		void SetVsync(bool vsync) const;

		void SetClearColor(float r, float g, float b) const;
		void Clear(ClearFlag clearFlag) const;
		void Swap() const;
		void SetViewport(float x, float y, float w, float h) const;
		void SetDisplayAsRenderTarget() const;

		static RenderContextGL* GetRenderContext();
	private:
		HDC m_hdc;

		static RenderContextGL* s_pRenderContext;
	};

} }