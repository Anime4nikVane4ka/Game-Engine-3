#include "ShootSystem.h"

#include "../../Config/Config.h"

#include <SFML/System/Time.hpp>

void ShootSystem::OnInit()
{
    Config config("config.txt");
    _bulletSpeed = config.getFloat("bullet_speed");

    _clock.restart();
}

void ShootSystem::OnUpdate()
{
    const float deltaTimeMs = static_cast<float>(_clock.restart().asMilliseconds());
    for (const int shooterEntity : _shooterComponents.Entities())
        _shooterComponents.Get(shooterEntity).Update(deltaTimeMs);

    for (const auto eventEntity : _shootEvents)
    {
        auto& shootEvent = _shootInputEvents.Get(eventEntity);
        const int shooterEntity = shootEvent.Entity;

        if (shootEvent.Shoot
            && _shooterComponents.Has(shooterEntity)
            && _positions.Has(shooterEntity))
        {
            auto& shooter = _shooterComponents.Get(shooterEntity); // ��������� ���������� ��������
            if (shooter.CanShoot)
            {
                const auto& shooterPosition = _positions.Get(shooterEntity);
                const int bulletEntity = world.CreateEntity();

                _positions.Add(bulletEntity, PositionComponent(
                    shooterPosition.Position.x,
                    shooterPosition.Position.y));
                _movements.Add(bulletEntity, MovementComponent(_bulletSpeed, 0.0f, -1.0f));
                _boxColliders.Add(bulletEntity, BoxColliderComponent(6.0f, 16.0f));
                _rectangleShapes.Add(bulletEntity, RectangleShapeComponent(6.0f, 16.0f, sf::Color::Yellow));
                _collisions.Add(bulletEntity, CollisionComponent());
                _bullets.Add(bulletEntity, BulletComponent(shooterEntity));

                shooter.Shot();
            }
        }

        world.RemoveEntity(eventEntity);
    }
}