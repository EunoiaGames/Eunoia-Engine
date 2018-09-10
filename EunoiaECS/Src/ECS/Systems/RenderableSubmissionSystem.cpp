#include "RenderableSubmissionSystem.h"
#include "../Components/MeshComponent.h"
#include "../Components/TransformComponent.h"
#include "../ECSEngine.h"

namespace Eunoia { namespace ECS {

	RenderableSubmissionSystem::RenderableSubmissionSystem()
	{
		AddComponentType(MeshComponent::COMPONENT_TYPE_ID);
		AddComponentType(TransformComponent::COMPONENT_TYPE_ID);
	}

	void RenderableSubmissionSystem::ProcessEntity(EntityID entityID, float dt)
	{
		MeshComponent* pMeshComponent = m_pECS->GetComponentByType<MeshComponent>(entityID);
		TransformComponent* pTransformComponent = m_pECS->GetComponentByType<TransformComponent>(entityID);

		Rendering::Renderable renderable;
		renderable.materialID = pMeshComponent->materialID;
		renderable.meshID = pMeshComponent->meshID;
		renderable.transform = pTransformComponent->transform.ToMatrix();

		m_subEngines.pRenderingEngine->SubmitRenderable(renderable);
	}

} }
