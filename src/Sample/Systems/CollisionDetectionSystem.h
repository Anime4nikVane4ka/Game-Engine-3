#ifndef COLLISIONDETECTIONSYSTEM_H
#define COLLISIONDETECTIONSYSTEM_H

#include "../../Ecs/Filter/Filter.h"
#include "../../Ecs/Filter/FilterBuilder.h"
#include "../../Ecs/Systems/ISystem.h"

#include "../Components/BoxColliderComponent.h"
#include "../Components/CircleColliderComponent.h"
#include "../Components/CollisionComponent.h"
#include "../Components/PositionComponent.h"

class CollisionDetectionSystem final : public ISystem
{
    ComponentStorage<PositionComponent>& _positions;
    ComponentStorage<CollisionComponent>& _collisions;
    ComponentStorage<BoxColliderComponent>& _boxColliders;
    ComponentStorage<CircleColliderComponent>& _circleColliders;

    Filter _collidableEntities;

    bool CheckCollision(int firstEntity, int secondEntity);
    bool CheckAabbVsAabb(int firstEntity, int secondEntity);
    bool CheckAabbVsCircle(int boxEntity, int circleEntity);
    bool CheckCircleVsCircle(int firstEntity, int secondEntity);

public:
    CollisionDetectionSystem(World& world)
        : ISystem(world),
          _positions(world.GetStorage<PositionComponent>()),
          _collisions(world.GetStorage<CollisionComponent>()),
          _boxColliders(world.GetStorage<BoxColliderComponent>()),
          _circleColliders(world.GetStorage<CircleColliderComponent>()),
          _collidableEntities(FilterBuilder(world)
              .With<PositionComponent>()
              .With<CollisionComponent>()
              .Build())
    {
    }

    void OnInit() override;
    void OnUpdate() override;
};

#endif //COLLISIONDETECTIONSYSTEM_H
