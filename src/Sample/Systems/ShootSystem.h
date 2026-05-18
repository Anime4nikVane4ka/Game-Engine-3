#ifndef SHOOTSYSTEM_H
#define SHOOTSYSTEM_H

#include <SFML/System/Clock.hpp>

#include "../../Ecs/Filter/Filter.h"
#include "../../Ecs/Filter/FilterBuilder.h"
#include "../../Ecs/Systems/ISystem.h"

#include "../Components/BoxColliderComponent.h"
#include "../Components/BulletComponent.h"
#include "../Components/CollisionComponent.h"
#include "../Components/MovementComponent.h"
#include "../Components/PositionComponent.h"
#include "../Components/RectangleShapeComponent.h"
#include "../Components/ShooterComponent.h"
#include "../Components/ShootInputEvent.h"

class ShootSystem final : public ISystem
{
    ComponentStorage<ShootInputEvent>& _shootInputEvents;
    ComponentStorage<ShooterComponent>& _shooterComponents;

    // ���������� �������:
    ComponentStorage<PositionComponent>& _positions;
    ComponentStorage<MovementComponent>& _movements;
    ComponentStorage<BulletComponent>& _bullets;
    ComponentStorage<CollisionComponent>& _collisions;
    ComponentStorage<BoxColliderComponent>& _boxColliders;
    ComponentStorage<RectangleShapeComponent>& _rectangleShapes;

    Filter _shootEvents;
    sf::Clock _clock;
    float _bulletSpeed;

public:
    ShootSystem(World& world)
        : ISystem(world),
          _shootInputEvents(world.GetStorage<ShootInputEvent>()),
          _shooterComponents(world.GetStorage<ShooterComponent>()),
          _positions(world.GetStorage<PositionComponent>()),
          _movements(world.GetStorage<MovementComponent>()),
          _bullets(world.GetStorage<BulletComponent>()),
          _collisions(world.GetStorage<CollisionComponent>()),
          _boxColliders(world.GetStorage<BoxColliderComponent>()),
          _rectangleShapes(world.GetStorage<RectangleShapeComponent>()),
          _shootEvents(FilterBuilder(world)
              .With<ShootInputEvent>()
              .Build())
    {
    }

    void OnInit() override;
    void OnUpdate() override;
};

#endif //SHOOTSYSTEM_H


