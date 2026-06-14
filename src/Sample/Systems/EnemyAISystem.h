#ifndef ENEMYAISYSTEM_H
#define ENEMYAISYSTEM_H

#include <SFML/System/Vector2.hpp>

#include "../../Ecs/Filter/Filter.h"
#include "../../Ecs/Filter/FilterBuilder.h"
#include "../../Ecs/Systems/ISystem.h"
#include "../Components/BoxColliderComponent.h"
#include "../Components/BrickTileComponent.h"
#include "../Components/GoombaComponent.h"
#include "../Components/PlayerComponent.h"
#include "../Components/PositionComponent.h"
#include "../Components/TileComponent.h"

class EnemyAISystem final : public ISystem {
    ComponentStorage<BoxColliderComponent>& _boxColliders;
    ComponentStorage<BrickTileComponent>& _bricks;
    ComponentStorage<GoombaComponent>& _goombas;
    ComponentStorage<PositionComponent>& _positions;
    ComponentStorage<TileComponent>& _tiles;

    Filter _goombaEntities;
    Filter _playerEntities;
    Filter _solidEntities;

    bool IsSolid(int entity) const;
    bool IsPlayerInView(int goombaEntity, int playerEntity);
    bool HasObstacleBetween(const sf::Vector2f& from, const sf::Vector2f& to);
    bool LineIntersectsBox(const sf::Vector2f& from, const sf::Vector2f& to, int boxEntity) const;

  public:
    EnemyAISystem(World& world)
        : ISystem(world), _boxColliders(world.GetStorage<BoxColliderComponent>()),
          _bricks(world.GetStorage<BrickTileComponent>()),
          _goombas(world.GetStorage<GoombaComponent>()),
          _positions(world.GetStorage<PositionComponent>()), _tiles(world.GetStorage<TileComponent>()),
          _goombaEntities(
              FilterBuilder(world).With<GoombaComponent>().With<PositionComponent>().Build()),
          _playerEntities(
              FilterBuilder(world).With<PlayerComponent>().With<PositionComponent>().Build()),
          _solidEntities(
              FilterBuilder(world).With<PositionComponent>().With<BoxColliderComponent>().Build()) {
    }

    void OnInit() override;
    void OnUpdate() override;
};

#endif // ENEMYAISYSTEM_H
