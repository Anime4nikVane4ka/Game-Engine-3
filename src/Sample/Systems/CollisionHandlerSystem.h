#ifndef COLLISIONHANDLERSYSTEM_H
#define COLLISIONHANDLERSYSTEM_H

#include <vector>

#include "../../Ecs/Filter/Filter.h"
#include "../../Ecs/Filter/FilterBuilder.h"
#include "../../Ecs/Systems/ISystem.h"

#include "../Components/AsteroidComponent.h"
#include "../Components/BulletComponent.h"
#include "../Components/CollisionComponent.h"
#include "../Components/CooldownBoostEvent.h"
#include "../Components/GameOverEvent.h"
#include "../Components/PlayerComponent.h"
#include "../Components/ScoreIncreaseEvent.h"
#include "../Components/UfoComponent.h"

class CollisionHandlerSystem final : public ISystem
{
    ComponentStorage<CollisionComponent>& _collisions;
    ComponentStorage<AsteroidComponent>& _asteroids;
    ComponentStorage<BulletComponent>& _bullets;
    ComponentStorage<PlayerComponent>& _players;
    ComponentStorage<ScoreIncreaseEvent>& _scoreIncreaseEvents;
    ComponentStorage<GameOverEvent>& _gameOverEvents;
    ComponentStorage<UfoComponent>& _ufos;
    ComponentStorage<CooldownBoostEvent>& _cooldownBoostEvents;

    Filter _collidableEntities;
    int _scorePerAsteroid = 1;
    float _ufoCooldownBoostDurationMs = 0.0f;

    bool HandleAsteroidCollision(int asteroidEntity, std::vector<int>& entitiesToRemove);
    bool HandleUfoCollision(int ufoEntity, std::vector<int>& entitiesToRemove);

public:
    CollisionHandlerSystem(World& world)
        : ISystem(world),
          _collisions(world.GetStorage<CollisionComponent>()),
          _asteroids(world.GetStorage<AsteroidComponent>()),
          _bullets(world.GetStorage<BulletComponent>()),
          _players(world.GetStorage<PlayerComponent>()),
          _scoreIncreaseEvents(world.GetStorage<ScoreIncreaseEvent>()),
          _gameOverEvents(world.GetStorage<GameOverEvent>()),
          _ufos(world.GetStorage<UfoComponent>()),
          _cooldownBoostEvents(world.GetStorage<CooldownBoostEvent>()),
          _collidableEntities(FilterBuilder(world)
              .With<CollisionComponent>()
              .Build())
    {
    }

    void OnInit() override;
    void OnUpdate() override;
};

#endif //COLLISIONHANDLERSYSTEM_H
