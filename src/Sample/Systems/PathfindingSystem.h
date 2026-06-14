#ifndef PATHFINDINGSYSTEM_H
#define PATHFINDINGSYSTEM_H

#include <vector>

#include <SFML/System/Vector2.hpp>

#include "../../Ecs/Filter/Filter.h"
#include "../../Ecs/Filter/FilterBuilder.h"
#include "../../Ecs/Systems/ISystem.h"
#include "../Components/BoxColliderComponent.h"
#include "../Components/BrickTileComponent.h"
#include "../Components/GoombaComponent.h"
#include "../Components/MovementComponent.h"
#include "../Components/PathfindingComponent.h"
#include "../Components/PlayerComponent.h"
#include "../Components/PositionComponent.h"
#include "../Components/TileComponent.h"

struct PathNode {
    sf::Vector2i Cell = {0, 0};
    int ParentIndex = -1;
    int Cost = 0;
    int TotalCost = 0;
    bool Closed = false;
};

class PathfindingSystem final : public ISystem {
    ComponentStorage<BrickTileComponent>& _bricks;
    ComponentStorage<GoombaComponent>& _goombas;
    ComponentStorage<MovementComponent>& _movements;
    ComponentStorage<PathfindingComponent>& _pathfindings;
    ComponentStorage<PositionComponent>& _positions;
    ComponentStorage<TileComponent>& _tiles;

    Filter _pathfindingEntities;
    Filter _playerEntities;
    Filter _solidEntities;

    sf::Vector2i GetCell(const sf::Vector2f& position) const;
    sf::Vector2f GetCellCenter(const sf::Vector2i& cell) const;
    sf::Vector2i GetMaxCell() const;
    bool IsSolid(int entity) const;
    bool IsBlocked(const sf::Vector2i& cell) const;
    bool IsInsideGrid(const sf::Vector2i& cell, const sf::Vector2i& maxCell) const;
    int GetHeuristic(const sf::Vector2i& from, const sf::Vector2i& to) const;
    int FindNode(const std::vector<PathNode>& nodes, const sf::Vector2i& cell) const;
    int FindOpenNode(const std::vector<PathNode>& nodes) const;
    std::vector<sf::Vector2i> BuildPath(const std::vector<PathNode>& nodes, int nodeIndex) const;
    std::vector<sf::Vector2i> FindPath(const sf::Vector2i& start, const sf::Vector2i& target) const;
    void FollowPath(int entity, PathfindingComponent& pathfinding, const sf::Vector2i& targetCell);

  public:
    PathfindingSystem(World& world)
        : ISystem(world), _bricks(world.GetStorage<BrickTileComponent>()),
          _goombas(world.GetStorage<GoombaComponent>()),
          _movements(world.GetStorage<MovementComponent>()),
          _pathfindings(world.GetStorage<PathfindingComponent>()),
          _positions(world.GetStorage<PositionComponent>()), _tiles(world.GetStorage<TileComponent>()),
          _pathfindingEntities(FilterBuilder(world)
                                   .With<GoombaComponent>()
                                   .With<MovementComponent>()
                                   .With<PathfindingComponent>()
                                   .With<PositionComponent>()
                                   .Build()),
          _playerEntities(
              FilterBuilder(world).With<PlayerComponent>().With<PositionComponent>().Build()),
          _solidEntities(
              FilterBuilder(world).With<PositionComponent>().With<BoxColliderComponent>().Build()) {
    }

    void OnInit() override;
    void OnUpdate() override;
};

#endif // PATHFINDINGSYSTEM_H
