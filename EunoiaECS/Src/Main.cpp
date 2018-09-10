#include "Core\CoreEngine.h"
#include "Rendering\Vertex.h"
#include "Rendering\ResourceManager.h"
#include "ECS\Components\Components.h"
#include "ECS\Systems\Systems.h"

using namespace Eunoia;
using namespace Rendering;
using namespace Core;
using namespace ECS;
using namespace Math;

struct SpeedComponent : public Component<SpeedComponent>
{
	SpeedComponent(float speed) :
		speed(speed)
	{}

	float speed;
};

class MoveSystem : public System<MoveSystem>
{
public:
	MoveSystem()
	{
		AddComponentType(SpeedComponent::COMPONENT_TYPE_ID);
		AddComponentType(TransformComponent::COMPONENT_TYPE_ID);
	}

	void ProcessEntity(EntityID entity, float dt)
	{
		SpeedComponent* pSpeedComponent = m_pECS->GetComponentByType<SpeedComponent>(entity);
		TransformComponent* pTransformComponent = m_pECS->GetComponentByType<TransformComponent>(entity);

		const Vector3f& pos = pTransformComponent->transform.GetPos();
		pTransformComponent->transform.GetPos().Set(sinf(m_time), pos.GetY(), pos.GetZ());

		m_time += dt * pSpeedComponent->speed;
	}
private:
	float m_time;
};

class TestApplication : public Application
{
public:
	void Init() override;
	void ShutDown() override;
};

void TestApplication::Init()
{
	std::vector<Vertex> vertices;
	vertices.push_back(Vertex(Math::Vector3f(-0.5f, -0.5f, 0.0f), Math::Vector2f(0.0f, 1.0f)));
	vertices.push_back(Vertex(Math::Vector3f(-0.5f, 0.5f, 0.0f), Math::Vector2f(0.0f, 0.0f)));
	vertices.push_back(Vertex(Math::Vector3f(0.5f, 0.5f, 0.0f), Math::Vector2f(1.0f, 0.0f)));
	vertices.push_back(Vertex(Math::Vector3f(0.5f, -0.5f, 0.0f), Math::Vector2f(1.0f, 1.0f)));
	std::vector<uint32> indices = { 0, 1, 2, 0, 2, 3 };
	MeshInfo info(vertices, indices);

	MeshID mesh = ResourceManager::pMeshManager->CreateMesh(info);
	
	MaterialID brickMaterial = ResourceManager::pMaterialManager->CreateMaterial("Res/Textures/Bricks.jpg");

	m_pEcs->CreateSystem<RenderableSubmissionSystem>(RENDER_PIPELINE);
	m_pEcs->CreateSystem<MoveSystem>(UPDATE_PIPELINE);

	EntityID quad = m_pEcs->CreateEntity();
	m_pEcs->CreateComponent<TransformComponent>(quad);
	m_pEcs->CreateComponent<MeshComponent>(quad, mesh, brickMaterial);
	m_pEcs->CreateComponent<SpeedComponent>(quad, 1.0f);
}

void TestApplication::ShutDown()
{

}

int main()
{
	CoreEngine coreEngine(new TestApplication(), "Eunoia Game Engine", 800, 600);
	coreEngine.Start();
}