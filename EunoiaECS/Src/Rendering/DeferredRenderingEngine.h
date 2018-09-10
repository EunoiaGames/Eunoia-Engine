#pragma once

#include "RenderingEngine.h"

namespace Eunoia { namespace Rendering {

	class DeferredRenderingEngine : public RenderingEngine
	{
	public:
		DeferredRenderingEngine();
		~DeferredRenderingEngine();

		void Render() override;
	private:
		Rendering::ShaderID m_ambientShader;
	};

} }
