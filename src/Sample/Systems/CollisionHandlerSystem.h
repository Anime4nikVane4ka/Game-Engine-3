#ifndef COLLISIONHANDLERSYSTEM_H
#define COLLISIONHANDLERSYSTEM_H

#include <vector>

#include "../../Ecs/Filter/Filter.h"
#include "../../Ecs/Filter/FilterBuilder.h"
#include "../../Ecs/Systems/ISystem.h"
#include "../Components/BrickTileComponent.h"
#include "../Components/BulletComponent.h"
#include "../Components/CollisionComponent.h"
#include "../Components/FinishComponent.h"
#include "../Components/MovementComponent.h"
#include "../Components/PlayerComponent.h"
#include "../Components/PositionComponent.h"
#include "../Components/TileComponent.h"

class CollisionHandlerSystem final : public ISystem
{
    ComponentStorage<CollisionComponent>& _collisions;
    ComponentStorage<BrickTileComponent>& _bricks;
    ComponentStorage<BulletComponent>& _bullets;
    ComponentStorage<FinishComponent>& _finishes;
    ComponentStorage<MovementComponent>& _movements;
    ComponentStorage<PlayerComponent>& _players;
    ComponentStorage<PositionComponent>& _positions;
    ComponentStorage<TileComponent>& _tiles;

    Filter _collidableEntities;

    bool IsSolid(int entity) const;
    bool IsBelow(int firstEntity, int secondEntity) const;
    void HandlePlayerCollision(int playerEntity, int collidedEntity, std::vector<int>& entitiesToRemove);
    void HandleBulletCollision(int bulletEntity, int collidedEntity, std::vector<int>& entitiesToRemove);

public:
    CollisionHandlerSystem(World& world)
        : ISystem(world),
          _collisions(world.GetStorage<CollisionComponent>()),
          _bricks(world.GetStorage<BrickTileComponent>()),
          _bullets(world.GetStorage<BulletComponent>()),
          _finishes(world.GetStorage<FinishComponent>()),
          _movements(world.GetStorage<MovementComponent>()),
          _players(world.GetStorage<PlayerComponent>()),
          _positions(world.GetStorage<PositionComponent>()),
          _tiles(world.GetStorage<TileComponent>()),
          _collidableEntities(FilterBuilder(world)
              .With<CollisionComponent>()
              .Build())
    {
    }

    void OnInit() override;
    void OnUpdate() override;
};

#endif //COLLISIONHANDLERSYSTEM_H
