#include "CollisionHandlerSystem.h"

#include "../../Config/Config.h"

#include <vector>

void CollisionHandlerSystem::OnInit()
{
    Config config("config.txt");
    _ufoCooldownBoostDurationMs = config.getFloat("ufo_cooldown_boost_duration");
}

bool CollisionHandlerSystem::HandleAsteroidCollision(const int asteroidEntity, std::vector<int>& entitiesToRemove)
{
    const auto& collision = _collisions.Get(asteroidEntity);
    for (const int collidedEntity : collision.CollidedEntities)
    {
        if (_bullets.Has(collidedEntity))
        {
            const int playerEntity = _bullets.Get(collidedEntity).Owner;
            const int eventEntity = world.CreateEntity();
            _scoreIncreaseEvents.Add(eventEntity, ScoreIncreaseEvent(playerEntity, _scorePerAsteroid));

            entitiesToRemove.push_back(asteroidEntity);
            entitiesToRemove.push_back(collidedEntity);
            return true;
        }

        if (_players.Has(collidedEntity))
        {
            const int score = _players.Get(collidedEntity).Score;
            const int eventEntity = world.CreateEntity();
            _gameOverEvents.Add(eventEntity, GameOverEvent(collidedEntity, score));

            entitiesToRemove.push_back(asteroidEntity);
            entitiesToRemove.push_back(collidedEntity);
            return true;
        }
    }

    return false;
}

bool CollisionHandlerSystem::HandleUfoCollision(const int ufoEntity, std::vector<int>& entitiesToRemove)
{
    const auto& collision = _collisions.Get(ufoEntity);
    for (const int collidedEntity : collision.CollidedEntities)
    {
        if (!_bullets.Has(collidedEntity))
            continue;

        const int playerEntity = _bullets.Get(collidedEntity).Owner;
        const int eventEntity = world.CreateEntity();
        _cooldownBoostEvents.Add(eventEntity, CooldownBoostEvent(playerEntity, 0.5f, _ufoCooldownBoostDurationMs));

        entitiesToRemove.push_back(ufoEntity);
        entitiesToRemove.push_back(collidedEntity);
        return true;
    }

    return false;
}

void CollisionHandlerSystem::OnUpdate()
{
    std::vector<int> entitiesToRemove;

    for (const auto entity : _collidableEntities)
    {
        if (_asteroids.Has(entity))
            HandleAsteroidCollision(entity, entitiesToRemove);
        else if (_ufos.Has(entity))
            HandleUfoCollision(entity, entitiesToRemove);
    }

    for (const int entity : entitiesToRemove)
    {
        if (world.IsEntityAlive(entity))
            world.RemoveEntity(entity);
    }
}
