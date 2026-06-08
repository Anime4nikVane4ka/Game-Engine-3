#include "ShootSystem.h"

#include <SFML/System/Time.hpp>

void ShootSystem::OnInit()
{
    _clock.restart();
}

void ShootSystem::CreateBullet(const int shooterEntity, const ShooterComponent& shooter)
{
    const auto& shooterPosition = _positions.Get(shooterEntity);
    const int bulletEntity = world.CreateEntity();

    auto& bulletPosition = _positions.Add(bulletEntity, PositionComponent(
        shooterPosition.Position.x + shooter.DirectionX * 48.0f,
        shooterPosition.Position.y - 8.0f));
    bulletPosition.Scale = {4.0f, 4.0f};
    _movements.Add(bulletEntity, MovementComponent(_bulletSpeed, shooter.DirectionX, 0.0f));
    _sprites.Add(bulletEntity, SpriteComponent(_bulletTexture));
    _circleColliders.Add(bulletEntity, CircleColliderComponent(_bulletRadius));
    _collisions.Add(bulletEntity, CollisionComponent());
    _bullets.Add(bulletEntity, BulletComponent(shooterEntity));
}

void ShootSystem::OnUpdate()
{
    const float deltaTimeMs = _clock.restart().asMilliseconds();

    for (const int shooterEntity : _shooterEntities)
    {
        auto& shooter = _shooters.Get(shooterEntity);
        const auto& movement = _movements.Get(shooterEntity);

        if (movement.Direction.x != 0.0f)
            shooter.DirectionX = movement.Direction.x;

        shooter.Update(deltaTimeMs);
        if (!shooter.Shoot || !shooter.CanShoot)
            continue;

        CreateBullet(shooterEntity, shooter);
        shooter.Shot();
    }
}
