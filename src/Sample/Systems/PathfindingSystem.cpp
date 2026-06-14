#include "PathfindingSystem.h"

#include <algorithm>
#include <cmath>

#include "../../Config/LevelConfig.h"

void PathfindingSystem::OnInit() {}

sf::Vector2i PathfindingSystem::GetCell(const sf::Vector2f& position) const {
    return sf::Vector2i(static_cast<int>(position.x) / LevelConfig::CellSize,
        static_cast<int>(position.y) / LevelConfig::CellSize);
}

sf::Vector2f PathfindingSystem::GetCellCenter(const sf::Vector2i& cell) const {
    return sf::Vector2f(cell.x * LevelConfig::CellSize + LevelConfig::CellSize / 2.0f,
        cell.y * LevelConfig::CellSize + LevelConfig::CellSize / 2.0f);
}

sf::Vector2i PathfindingSystem::GetMaxCell() {
    sf::Vector2i maxCell = {0, 0};

    for (const int entity : _solidEntities) {
        const sf::Vector2i cell = GetCell(_positions.Get(entity).Position);
        if (cell.x > maxCell.x)
            maxCell.x = cell.x;
        if (cell.y > maxCell.y)
            maxCell.y = cell.y;
    }

    for (const int entity : _playerEntities) {
        const sf::Vector2i cell = GetCell(_positions.Get(entity).Position);
        if (cell.x > maxCell.x)
            maxCell.x = cell.x;
        if (cell.y > maxCell.y)
            maxCell.y = cell.y;
    }

    return sf::Vector2i(maxCell.x + 5, maxCell.y + 5);
}

bool PathfindingSystem::IsSolid(const int entity) const {
    return _tiles.Has(entity) || _bricks.Has(entity);
}

bool PathfindingSystem::IsBlocked(const sf::Vector2i& cell) {
    for (const int entity : _solidEntities) {
        if (!IsSolid(entity))
            continue;

        if (GetCell(_positions.Get(entity).Position) == cell)
            return true;
    }

    return false;
}

bool PathfindingSystem::IsInsideGrid(const sf::Vector2i& cell,
    const sf::Vector2i& maxCell) const {
    return cell.x >= 0 && cell.y >= 0 && cell.x <= maxCell.x && cell.y <= maxCell.y;
}

int PathfindingSystem::GetHeuristic(const sf::Vector2i& from, const sf::Vector2i& to) const {
    return (std::abs(to.x - from.x) + std::abs(to.y - from.y)) * 10;
}

int PathfindingSystem::FindNode(const std::vector<PathNode>& nodes,
    const sf::Vector2i& cell) const {
    for (int i = 0; i < nodes.size(); i++) {
        if (nodes[i].Cell == cell)
            return i;
    }

    return -1;
}

int PathfindingSystem::FindOpenNode(const std::vector<PathNode>& nodes) const {
    int bestIndex = -1;

    for (int i = 0; i < nodes.size(); i++) {
        if (nodes[i].Closed)
            continue;

        if (bestIndex == -1 || nodes[i].TotalCost < nodes[bestIndex].TotalCost)
            bestIndex = i;
    }

    return bestIndex;
}

std::vector<sf::Vector2i> PathfindingSystem::BuildPath(const std::vector<PathNode>& nodes,
    int nodeIndex) const {
    std::vector<sf::Vector2i> path;

    while (nodeIndex >= 0) {
        path.push_back(nodes[nodeIndex].Cell);
        nodeIndex = nodes[nodeIndex].ParentIndex;
    }

    std::reverse(path.begin(), path.end());
    return path;
}

std::vector<sf::Vector2i> PathfindingSystem::FindPath(const sf::Vector2i& start,
    const sf::Vector2i& target) {
    if (start == target)
        return {target};

    const sf::Vector2i maxCell = GetMaxCell();
    std::vector<PathNode> nodes;
    nodes.push_back(PathNode{start, -1, 0, GetHeuristic(start, target), false});

    const std::vector<sf::Vector2i> directions = {
        {-1, 0},
        {1, 0},
        {-1, -1},
        {1, -1},
        {-1, 1},
        {1, 1},
    };

    while (true) {
        const int currentIndex = FindOpenNode(nodes);
        if (currentIndex == -1)
            break;

        auto& currentNode = nodes[currentIndex];
        currentNode.Closed = true;

        if (currentNode.Cell == target)
            return BuildPath(nodes, currentIndex);

        for (const auto& direction : directions) {
            const sf::Vector2i nextCell = currentNode.Cell + direction;
            if (!IsInsideGrid(nextCell, maxCell) || IsBlocked(nextCell))
                continue;

            const int nextCost = currentNode.Cost + (direction.y == 0 ? 10 : 14);
            const int nodeIndex = FindNode(nodes, nextCell);

            if (nodeIndex == -1) {
                nodes.push_back(PathNode{nextCell,
                    currentIndex,
                    nextCost,
                    nextCost + GetHeuristic(nextCell, target),
                    false});
                continue;
            }

            if (nodes[nodeIndex].Closed || nextCost >= nodes[nodeIndex].Cost)
                continue;

            nodes[nodeIndex].ParentIndex = currentIndex;
            nodes[nodeIndex].Cost = nextCost;
            nodes[nodeIndex].TotalCost = nextCost + GetHeuristic(nextCell, target);
        }
    }

    return {};
}

void PathfindingSystem::FollowPath(const int entity,
    PathfindingComponent& pathfinding,
    const sf::Vector2i& targetCell) {
    auto& movement = _movements.Get(entity);
    const auto& position = _positions.Get(entity);
    const sf::Vector2i currentCell = GetCell(position.Position);

    if (pathfinding.NeedUpdate || pathfinding.Path.empty() ||
        pathfinding.Path.back() != targetCell) {
        pathfinding.Path = FindPath(currentCell, targetCell);
        pathfinding.CurrentPathIndex = 0;
        pathfinding.NeedUpdate = false;
    }

    if (pathfinding.Path.empty()) {
        movement.Direction.x = 0.0f;
        return;
    }

    while (pathfinding.CurrentPathIndex < pathfinding.Path.size() &&
           pathfinding.Path[pathfinding.CurrentPathIndex] == currentCell) {
        pathfinding.CurrentPathIndex++;
    }

    if (pathfinding.CurrentPathIndex >= pathfinding.Path.size()) {
        movement.Direction.x = 0.0f;
        return;
    }

    const sf::Vector2i nextCell = pathfinding.Path[pathfinding.CurrentPathIndex];
    const sf::Vector2f targetPosition = GetCellCenter(nextCell);
    const float distanceX = targetPosition.x - position.Position.x;

    if (std::abs(distanceX) <= movement.Speed)
        movement.Direction.x = 0.0f;
    else if (distanceX > 0.0f)
        movement.Direction.x = 1.0f;
    else
        movement.Direction.x = -1.0f;

    if (nextCell.y < currentCell.y && movement.IsGrounded) {
        movement.Direction.y = -movement.JumpForce;
        movement.IsGrounded = false;
    }
}

void PathfindingSystem::OnUpdate() {
    int playerEntity = -1;
    for (const int entity : _playerEntities) {
        playerEntity = entity;
        break;
    }

    if (playerEntity == -1)
        return;

    const sf::Vector2i playerCell = GetCell(_positions.Get(playerEntity).Position);

    for (const int entity : _pathfindingEntities) {
        auto& pathfinding = _pathfindings.Get(entity);

        if (!_goombas.Get(entity).IsChasing) {
            pathfinding.Path.clear();
            pathfinding.CurrentPathIndex = 0;
            pathfinding.NeedUpdate = true;
            continue;
        }

        FollowPath(entity, pathfinding, playerCell);
    }
}
