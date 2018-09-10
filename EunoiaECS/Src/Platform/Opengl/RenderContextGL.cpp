#include "RenderContextGL.h"

namespace Eunoia { namespace Rendering {

	RenderContextGL* RenderContextGL::s_pRenderContext = NULL;

	RenderContextGL::RenderContextGL()
	{
		m_hdc = GetDC(Win32Display::GetDisplay()->GetWindowHandle());
		HGLRC glrc = wglCreateContext(m_hdc);

		if (glrc)
		{
			if (!wglMakeCurrent(m_hdc, glrc))
			{
				EU_FORCE_SHUTDOWN("Could not create opengl context");
			}

			std::cout << "Successfully created opengl context" << std::endl;
		}
		else
		{
			EU_FORCE_SHUTDOWN("Could not create opengl context");
		}

		if (glewInit() != GLEW_OK)
		{
			EU_FORCE_SHUTDOWN("Could not initialize GLEW");
		}

		std::cout << "Initialized GLEW" << std::endl;

		SetVsync(true);
		s_pRenderContext = this;
	}

	RenderContextGL::~RenderContextGL()
	{
		wglMakeCurrent(NULL, NULL);
		wglDeleteContext(wglCreateContext(m_hdc));
	}

	void RenderContextGL::SetVsync(bool vsync) const
	{
		wglSwapIntervalEXT(vsync ? 1 : 0);
	}

	void RenderContextGL::SetClearColor(float r, float g, float b) const
	{
		glClearColor(r, g, b, 1.0f);
	}

	void RenderContextGL::Clear(ClearFlag clearFlag) const
	{
		glClear(clearFlag);
	}

	void RenderContextGL::Swap() const
	{
		SwapBuffers(m_hdc);
	}

	void RenderContextGL::SetViewport(float x, float y, float w, float h) const
	{
		glViewport(x, y, w, h);
	}

	void RenderContextGL::SetDisplayAsRenderTarget() const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, Win32Display::GetDisplay()->GetWidth(), Win32Display::GetDisplay()->GetHeight());
	}

	RenderContextGL * RenderContextGL::GetRenderContext()
	{
		return s_pRenderContext;
	}

} }
