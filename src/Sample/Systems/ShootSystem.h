#ifndef SHOOTSYSTEM_H
#define SHOOTSYSTEM_H

#include <SFML/System/Clock.hpp>
#include <SFML/Graphics/Texture.hpp>

#include "../../Ecs/Filter/Filter.h"
#include "../../Ecs/Filter/FilterBuilder.h"
#include "../../Ecs/Systems/ISystem.h"
#include "../Components/BulletComponent.h"
#include "../Components/CircleColliderComponent.h"
#include "../Components/CollisionComponent.h"
#include "../Components/MovementComponent.h"
#include "../Components/PositionComponent.h"
#include "../Components/ShooterComponent.h"
#include "../Components/SpriteComponent.h"

class ShootSystem final : public ISystem
{
    ComponentStorage<BulletComponent>& _bullets;
    ComponentStorage<CircleColliderComponent>& _circleColliders;
    ComponentStorage<CollisionComponent>& _collisions;
    ComponentStorage<MovementComponent>& _movements;
    ComponentStorage<PositionComponent>& _positions;
    ComponentStorage<ShooterComponent>& _shooters;
    ComponentStorage<SpriteComponent>& _sprites;

    Filter _shooterEntities;
    sf::Clock _clock;
    const sf::Texture& _bulletTexture;
    float _bulletRadius = 0.0f;

    void CreateBullet(int shooterEntity, const ShooterComponent& shooter);

public:
    ShootSystem(
        World& world,
        const sf::Texture& bulletTexture,
        const float bulletRadius)
        : ISystem(world),
          _bullets(world.GetStorage<BulletComponent>()),
          _circleColliders(world.GetStorage<CircleColliderComponent>()),
          _collisions(world.GetStorage<CollisionComponent>()),
          _movements(world.GetStorage<MovementComponent>()),
          _positions(world.GetStorage<PositionComponent>()),
          _shooters(world.GetStorage<ShooterComponent>()),
          _sprites(world.GetStorage<SpriteComponent>()),
          _shooterEntities(FilterBuilder(world)
              .With<ShooterComponent>()
              .With<PositionComponent>()
              .With<MovementComponent>()
              .Build()),
          _bulletTexture(bulletTexture),
          _bulletRadius(bulletRadius)
    {
    }

    void OnInit() override;
    void OnUpdate() override;
};

#endif //SHOOTSYSTEM_H
