#include "CollisionHandlerSystem.h"

void CollisionHandlerSystem::OnInit()
{
}

bool CollisionHandlerSystem::IsSolid(const int entity) const
{
    return _tiles.Has(entity) || _bricks.Has(entity);
}

bool CollisionHandlerSystem::IsBelow(const int firstEntity, const int secondEntity) const
{
    if (!_positions.Has(firstEntity) || !_positions.Has(secondEntity))
        return false;

    return _positions.Get(firstEntity).Position.y > _positions.Get(secondEntity).Position.y;
}

void CollisionHandlerSystem::HandlePlayerCollision(
    const int playerEntity,
    const int collidedEntity,
    std::vector<int>& entitiesToRemove)
{
    if (_finishes.Has(collidedEntity))
    {
        if (_movements.Has(playerEntity))
            _movements.Get(playerEntity).Direction = {0.0f, 0.0f};
        return;
    }

    if (!IsSolid(collidedEntity) || !_movements.Has(playerEntity))
        return;

    auto& movement = _movements.Get(playerEntity);
    if (movement.Direction.y > 0.0f && IsBelow(playerEntity, collidedEntity))
    {
        movement.Direction.y = 0.0f;
        return;
    }

    if (movement.Direction.y < 0.0f && !IsBelow(playerEntity, collidedEntity))
    {
        movement.Direction.y = 0.0f;
        if (_bricks.Has(collidedEntity))
            entitiesToRemove.push_back(collidedEntity);
    }
}

void CollisionHandlerSystem::HandleBulletCollision(
    const int bulletEntity,
    const int collidedEntity,
    std::vector<int>& entitiesToRemove)
{
    if (!_bricks.Has(collidedEntity))
        return;

    entitiesToRemove.push_back(bulletEntity);
    entitiesToRemove.push_back(collidedEntity);
}

void CollisionHandlerSystem::OnUpdate()
{
    std::vector<int> entitiesToRemove;

    for (const int entity : _collidableEntities)
    {
        const auto& collision = _collisions.Get(entity);
        for (const int collidedEntity : collision.CollidedEntities)
        {
            if (_players.Has(entity))
                HandlePlayerCollision(entity, collidedEntity, entitiesToRemove);

            if (_bullets.Has(entity))
                HandleBulletCollision(entity, collidedEntity, entitiesToRemove);
        }
    }

    for (const int entity : entitiesToRemove)
    {
        if (world.IsEntityAlive(entity))
            world.RemoveEntity(entity);
    }
}
