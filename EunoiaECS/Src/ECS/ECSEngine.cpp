#include "ECSEngine.h"

namespace Eunoia { namespace ECS {

	ECSEngine::ECSEngine() :
		m_componentStackAllocator(100000000),
		m_systemStackAllocator(100000000)
	{

	}

	ECSEngine::~ECSEngine()
	{
	}

	EntityID ECSEngine::CreateEntity()
	{
		if (!m_freeEntityIDs.empty())
		{
			EntityID id = m_freeEntityIDs[m_freeEntityIDs.size() - 1];
			m_freeEntityIDs.pop_back();
			return id;
		}
		else
		{
			return m_lastEntityID++;
		}
	}

	void ECSEngine::DestroyEntity(EntityID entityID)
	{
		std::vector<ComponentContainer>& components = m_entitiesComponents.at(entityID);


		for (uint32 i = 0; i < components.size(); i++)
		{
			m_componentTypeAllocators[components[i].componentType].Free(components[i].pComponent);
		}

		m_freeEntityIDs.push_back(entityID);
	}

	ComponentContainer * ECSEngine::GetComponentList(EntityID entityID, uint32 * pNumComponents)
	{
		std::vector<ComponentContainer>& components = m_entitiesComponents[entityID];
		*pNumComponents = components.size();
		return &components[0];
	}

	BaseComponent * ECSEngine::GetComponent(EntityID entityID, ComponentID componentID)
	{
		return m_entitiesComponents.at(entityID)[componentID].pComponent;
	}

	void ECSEngine::RemoveComponent(EntityID entityID, ComponentID componentID)
	{
		m_componentTypeAllocators[entityID].Free(m_entitiesComponents.at(entityID)[componentID].pComponent);
		m_freeComponentIDs[entityID].push_back(componentID);
	}

	void ECSEngine::UpdatePipeline(SystemPipeLineType pipelineType, float dt)
	{
		SystemPipeline& pipeline = m_systemPipelines[pipelineType];

		for (auto& it : pipeline)
		{
			BaseSystem* pSystem = it.second;

			if (!pSystem->IsEnabled())
				continue;

			pSystem->PreProcess(dt);

			SystemSignature& componentTypes = pSystem->GetSignature();

			for (auto& entities : m_entitiesComponents)
			{
				EntityID entityID = entities.first;
				std::vector<ComponentContainer>& components = entities.second;
				
				if (IsEntityValidForSystem(components, componentTypes))
					pSystem->ProcessEntity(entityID, dt);
			}

			pSystem->PostProcess(dt);
		}
	}

	bool ECSEngine::IsEntityValidForSystem(std::vector<ComponentContainer>& entityComponents, std::vector<ComponentTypeID>& required)
	{
		for (uint32 i = 0; i < required.size(); i++)
		{
			if (!ContainsComponent(entityComponents, required[i]))
				return false;
		}

		return true;
	}

	bool ECSEngine::ContainsComponent(std::vector<ComponentContainer>& entityComponents, ComponentTypeID componentID)
	{
		for (uint32 i = 0; i < entityComponents.size(); i++)
		{
			if (componentID == entityComponents[i].componentType)
				return true;
		}

		return false;
	}

} }
