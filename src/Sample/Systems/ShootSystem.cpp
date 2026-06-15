#include "ShootSystem.h"

#include <vector>

#include <SFML/System/Time.hpp>

void ShootSystem::OnInit() {
    _clock.restart();
}

void ShootSystem::CreateBullet(const int shooterEntity, const ShooterComponent& shooter) {
    const auto& shooterPosition = _positions.Get(shooterEntity);
    const int bulletEntity = world.CreateEntity();

    auto& bulletPosition = _positions.Add(bulletEntity, PositionComponent(shooterPosition.Position.x + shooter.DirectionX * 48.0f, shooterPosition.Position.y - 8.0f));
    auto& bullet = _bullets.Add(bulletEntity, BulletComponent(shooterEntity, shooter.BulletSpeed));
    _movements.Add(bulletEntity, MovementComponent(bullet.Speed, shooter.DirectionX, 0.0f));
    auto& sprite = _sprites.Add(bulletEntity, SpriteComponent(_bulletTexture));
    bulletPosition.Scale = {_bulletRadius * 2.0f / sprite.TextureRect.size.x, _bulletRadius * 2.0f / sprite.TextureRect.size.y};
    _circleColliders.Add(bulletEntity, CircleColliderComponent(_bulletRadius));
    _collisions.Add(bulletEntity, CollisionComponent());
}

void ShootSystem::UpdateBullets(const float deltaTimeMs) {
    std::vector<int> bulletsToRemove;

    for (const int bulletEntity : _bulletEntities) {
        auto& bullet = _bullets.Get(bulletEntity);
        bullet.TimeToLiveMs -= deltaTimeMs;

        if (bullet.TimeToLiveMs <= 0.0f)
            bulletsToRemove.push_back(bulletEntity);
    }

    for (const int bulletEntity : bulletsToRemove) {
        if (world.IsEntityAlive(bulletEntity))
            world.RemoveEntity(bulletEntity);
    }
}

void ShootSystem::OnUpdate() {
    const float deltaTimeMs = _clock.restart().asMilliseconds();

    UpdateBullets(deltaTimeMs);

    for (const int shooterEntity : _shooterEntities) {
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
