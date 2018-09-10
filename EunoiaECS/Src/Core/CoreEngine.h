#pragma once

#include "Application.h"
#include "../Rendering/Display.h"
#include "../Rendering/RenderContext.h"
#include "../Rendering/RenderingEngine.h"
#include "SubEngines.h"

namespace Eunoia { namespace Core {

	class CoreEngine
	{
	public:
		CoreEngine(Application* pApplication, const String& title, uint32 width, uint32 height);
		~CoreEngine();

		void Start();
		void Stop();

		Rendering::RenderingEngine* GetRenderingEngine() const;
	private:
		void Update(float dt);
		void Render();
	private:
		Application* m_pApplication;
		Rendering::Display m_display;
		ECS::ECSEngine m_ecs;
		Rendering::RenderContext m_renderContext;
		Rendering::RenderingEngine* m_pRenderingEngine;
		
		bool m_running;
	};

} }
