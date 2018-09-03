#include "ECS\ECSEngine.h"
#include "ECS\Component.h"
#include "ECS\System.h"

using namespace Eunoia;
using namespace ECS;

struct PositionComponent : public Component<PositionComponent>
{
	PositionComponent(float x, float y, float z) :
		x(x), y(y), z(z)
	{}

	float x;
	float y;
	float z;
};

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
		AddComponentType(PositionComponent::COMPONENT_TYPE_ID);
		AddComponentType(SpeedComponent::COMPONENT_TYPE_ID);
	}

	void UpdateEntity(EntityID entity, float dt) override
	{
		PositionComponent* pPositionComponent = m_pECS->GetComponentByType<PositionComponent>(entity);
		SpeedComponent* pSpeedComponent = m_pECS->GetComponentByType<SpeedComponent>(entity);

		pPositionComponent->x += pSpeedComponent->speed * dt;
		pPositionComponent->y += pSpeedComponent->speed * dt;
		pPositionComponent->z += pSpeedComponent->speed * dt;
	}
};

int main()
{
	uint32 i = 0;
}