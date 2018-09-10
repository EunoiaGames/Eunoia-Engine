#include "DeferredRenderingEngine.h"
#include "ResourceManager.h"

namespace Eunoia { namespace Rendering {

	DeferredRenderingEngine::DeferredRenderingEngine()
	{
		m_ambientShader = Rendering::ResourceManager::pShaderManager->CreateShader("Test");
	}

	DeferredRenderingEngine::~DeferredRenderingEngine()
	{

	}

	void DeferredRenderingEngine::Render()
	{
		Rendering::ResourceManager::pShaderManager->BindShader(m_ambientShader);

		//TODO : Set camera matrices

		MaterialID lastMaterialID = EU_INVALID_MATERIAL_ID;
		for (uint32 i = 0; i < m_renderables.size(); i++)
		{
			//TODO : Set world matrix

			if(m_renderables[i].materialID != lastMaterialID)
				Rendering::ResourceManager::pMaterialManager->BindMaterialToShader(m_renderables[i].materialID, m_ambientShader);

			Rendering::ResourceManager::pMeshManager->RenderMesh(m_renderables[i].meshID);

			lastMaterialID = m_renderables[i].materialID;
		}
	}

} }
