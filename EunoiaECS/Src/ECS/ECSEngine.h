#pragma once

#include "BaseComponent.h"
#include "SystemPipeline.h"
#include "../Memory/PoolAllocator.h"
#include "../Memory/StackAllocator.h"
#include <unordered_map>
#include <vector>

namespace Eunoia { namespace Core { class CoreEngine; } }

namespace Eunoia { namespace ECS {

	struct ComponentContainer
	{
		BaseComponent* pComponent;
		ComponentTypeID componentType;
	};

	class ECSEngine
	{
	public:
		ECSEngine();
		~ECSEngine();

		EntityID CreateEntity();

		void DestroyEntity(EntityID entityID);

		ComponentContainer* GetComponentList(EntityID entityID, uint32* pNumComponents);

		template<class C, class... Args>
		ComponentID CreateComponent(EntityID entityID, Args&&... args)
		{
			const ComponentTypeID typeID = C::COMPONENT_TYPE_ID;
			const uint32 numComponentsPerType = C::MAX_INSTANCES;

			if (m_componentTypeAllocators.find(typeID) == m_componentTypeAllocators.end())
			{
				uint32 memorySize = numComponentsPerType * sizeof(C);
				PoolAllocator componentTypeAllocator(m_componentStackAllocator.Allocate(memorySize), memorySize, numComponentsPerType);
				m_componentTypeAllocators[typeID] = componentTypeAllocator;
			}

			void* pComponentMemory = m_componentTypeAllocators[typeID].Allocate();
			BaseComponent* pComponent = new(pComponentMemory) C(std::forward<Args>(args)...);

			ComponentContainer container;
			container.pComponent = pComponent;
			container.componentType = typeID;

			std::vector<ComponentID>& freeComponentIDs = m_freeComponentIDs[entityID];
			if (!freeComponentIDs.empty())
			{
				ComponentID componentID = freeComponentIDs.back();
				m_entitiesComponents[entityID][componentID] = container;
				freeComponentIDs.pop_back();
				return componentID;
			}
			else
			{
				std::vector<ComponentContainer>& components = m_entitiesComponents[entityID];
				components.push_back(container);
				return components.size() - 1;
			}
		}

		template<class C>
		C* GetComponentByType(EntityID entityID)
		{
			ComponentTypeID typeID = C::COMPONENT_TYPE_ID;
			std::vector<ComponentContainer>& components = m_entitiesComponents.at(entityID);
			for (uint32 i = 0; i < components.size(); i++)
			{
				if (components[i].componentType == typeID)
				{
					return (C*)components[i].pComponent;
				}
			}
		}

		// Fastest way to obtain a component
		BaseComponent* GetComponent(EntityID entityID, ComponentID componentID);

		template<class C>
		void RemoveComponentByType(EntityID entityID)
		{
			ComponentTypeID typeID = C::COMPONENT_TYPE_ID;
			std::vector<ComponentContainer>& components = m_entitiesComponents.at(entityID);
			for (uint32 i = 0; i < components.size(); i++)
			{
				if (components[i].componentType == typeID)
				{
					m_componentTypeAllocators[components[i].componentType].Free(components[i].pComponent);
					m_freeComponentIDs[entityID].push_back(i);
					return;
				}
			}
		}

		// Fastest way to Remove a component
		void RemoveComponent(EntityID entityID, ComponentID componentID);

		template<class S, class... Args>
		S* CreateSystem(SystemPipeLineType pipelineType, Args&&... args)
		{
			SystemTypeID typeID = S::SYSTEM_TYPE_ID;
			
			void* pSystemMemory = m_systemStackAllocator.Allocate(sizeof(S));
			BaseSystem* pSystem = new(pSystemMemory) S(std::forward<Args>(args)...);
			pSystem->m_pECS = this;
			pSystem->m_subEngines = m_subEngines;

			m_systemPipelines[pipelineType][typeID] = pSystem;

			return (S*)pSystem;
		}

		template<class S>
		void GetSystem(SystemPipeLineType pipelineType)
		{
			SystemTypeID typeID = S::SYSTEM_TYPE_ID;
			return m_systemPipelines[pipelineType][typeID];
		}

		void UpdatePipeline(SystemPipeLineType pipelineType, float dt);

	private:
		bool IsEntityValidForSystem(std::vector<ComponentContainer>& entityComponents, std::vector<ComponentTypeID>& required);
		bool ContainsComponent(std::vector<ComponentContainer>& entityComponents, ComponentTypeID componentID);
	private:
		friend Core::CoreEngine;
		SubEngines m_subEngines;

		std::unordered_map<ComponentTypeID, PoolAllocator> m_componentTypeAllocators;
		std::unordered_map<EntityID, std::vector<ComponentContainer>> m_entitiesComponents;
		std::unordered_map<EntityID, std::vector<ComponentID>> m_freeComponentIDs;

		SystemPipeline m_systemPipelines[NUM_SYSTEM_PIPELINE_TYPES];
		
		StackAllocator m_componentStackAllocator;
		StackAllocator m_systemStackAllocator;

		EntityID m_lastEntityID;
		ComponentID m_lastComponentID;

		std::vector<EntityID> m_freeEntityIDs;
	};

} }
