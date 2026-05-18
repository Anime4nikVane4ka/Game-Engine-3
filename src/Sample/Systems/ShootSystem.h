#ifndef SHOOTSYSTEM_H
#define SHOOTSYSTEM_H

#include <memory>
#include <utility>

#include <SFML/System/Clock.hpp>

#include "../../Ecs/Filter/Filter.h"
#include "../../Ecs/Filter/FilterBuilder.h"
#include "../../Ecs/Systems/ISystem.h"
#include "../../GameEngine/Input/InputAction.h"
#include "../Components/BulletComponent.h"
#include "../Components/CircleColliderComponent.h"
#include "../Components/CollisionComponent.h"
#include "../Components/MovementComponent.h"
#include "../Components/PositionComponent.h"
#include "../Components/ShooterComponent.h"

class ShootSystem final : public ISystem
{
    ComponentStorage<BulletComponent>& _bullets;
    ComponentStorage<CircleColliderComponent>& _circleColliders;
    ComponentStorage<CollisionComponent>& _collisions;
    ComponentStorage<MovementComponent>& _movements;
    ComponentStorage<PositionComponent>& _positions;
    ComponentStorage<ShooterComponent>& _shooters;

    Filter _shooterEntities;
    sf::Clock _clock;
    std::shared_ptr<InputAction> _shootAction;
    float _bulletSpeed = 0.0f;
    float _bulletRadius = 0.0f;

    bool IsShootActive() const;
    void CreateBullet(int shooterEntity, const ShooterComponent& shooter);

public:
    ShootSystem(
        World& world,
        std::shared_ptr<InputAction> shootAction,
        const float bulletSpeed,
        const float bulletRadius)
        : ISystem(world),
          _bullets(world.GetStorage<BulletComponent>()),
          _circleColliders(world.GetStorage<CircleColliderComponent>()),
          _collisions(world.GetStorage<CollisionComponent>()),
          _movements(world.GetStorage<MovementComponent>()),
          _positions(world.GetStorage<PositionComponent>()),
          _shooters(world.GetStorage<ShooterComponent>()),
          _shooterEntities(FilterBuilder(world)
              .With<ShooterComponent>()
              .With<PositionComponent>()
              .With<MovementComponent>()
              .Build()),
          _shootAction(std::move(shootAction)),
          _bulletSpeed(bulletSpeed),
          _bulletRadius(bulletRadius)
    {
    }

    void OnInit() override;
    void OnUpdate() override;
};

#endif //SHOOTSYSTEM_H
