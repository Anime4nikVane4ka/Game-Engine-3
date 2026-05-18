#include "ShootSystem.h"

void ShootSystem::OnInit()
{
    _clock.restart();
}

bool ShootSystem::IsShootActive() const
{
    return _shootAction != nullptr && _shootAction->Type() == Start;
}

void ShootSystem::CreateBullet(const int shooterEntity, const ShooterComponent& shooter)
{
    const auto& shooterPosition = _positions.Get(shooterEntity);
    const int bulletEntity = world.CreateEntity();

    _positions.Add(bulletEntity, PositionComponent(
        shooterPosition.Position.x,
        shooterPosition.Position.y));
    _movements.Add(bulletEntity, MovementComponent(_bulletSpeed, shooter.DirectionX, 0.0f));
    _circleColliders.Add(bulletEntity, CircleColliderComponent(_bulletRadius));
    _collisions.Add(bulletEntity, CollisionComponent());
    _bullets.Add(bulletEntity, BulletComponent(shooterEntity));
}

void ShootSystem::OnUpdate()
{
    const float deltaTimeMs = static_cast<float>(_clock.restart().asMilliseconds());
    const bool shootActive = IsShootActive();

    for (const int shooterEntity : _shooterEntities)
    {
        auto& shooter = _shooters.Get(shooterEntity);
        const auto& movement = _movements.Get(shooterEntity);

        if (movement.Direction.x != 0.0f)
            shooter.DirectionX = movement.Direction.x;

        shooter.Update(deltaTimeMs);
        if (!shootActive || !shooter.CanShoot)
            continue;

        CreateBullet(shooterEntity, shooter);
        shooter.Shot();
    }
}
