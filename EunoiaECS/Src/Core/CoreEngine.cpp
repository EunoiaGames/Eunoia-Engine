#include "CoreEngine.h"
#include "../Rendering/RenderContext.h"
#include "../Rendering/ResourceManager.h"
#include "../Rendering/DeferredRenderingEngine.h"

namespace Eunoia { namespace Core {

	Eunoia::Core::CoreEngine::CoreEngine(Application * pApplication, const String & title, uint32 width, uint32 height) :
		m_display(title, width, height),
		m_pApplication(pApplication)
	{
		m_renderContext.SetViewport(0.0f, 0.0f, width, height);
		Rendering::ResourceManager::InitResourceManager();
		m_pRenderingEngine = new Rendering::DeferredRenderingEngine();
		m_running = false;
	}

	CoreEngine::~CoreEngine()
	{
		delete m_pApplication;
	}

	void CoreEngine::Start()
	{
		if (m_running)
			return;

		m_running = true;
		m_ecs.m_subEngines.pRenderingEngine = m_pRenderingEngine;
		m_pApplication->m_pEcs = &m_ecs;
		m_pApplication->Init();

		while (m_running)
		{
			if (m_display.ShouldClose())
				Stop();

			Update(1.0f / 60.0f);
			Render();
		}

		m_pApplication->ShutDown();
		Rendering::ResourceManager::DestroyResourceManager();
	}

	void CoreEngine::Stop()
	{
		if (!m_running)
			return;

		m_running = false;
	}

	Rendering::RenderingEngine * CoreEngine::GetRenderingEngine() const
	{
		return m_pRenderingEngine;
	}

	void CoreEngine::Update(float dt)
	{
		m_ecs.UpdatePipeline(ECS::UPDATE_PIPELINE, dt);
	}

	void CoreEngine::Render()
	{
		m_renderContext.SetDisplayAsRenderTarget();
		m_renderContext.Clear(CLEAR_FLAG_COLOR_BUFFER | CLEAR_FLAG_DEPTH_BUFFER);
		m_pRenderingEngine->PreRender();
		m_ecs.UpdatePipeline(ECS::RENDER_PIPELINE, 0.0f);
		m_pRenderingEngine->Render();
		m_display.Update();
		m_renderContext.Swap();
	}

} }
