#include "EnemyAISystem.h"

void EnemyAISystem::OnInit() {}

bool EnemyAISystem::IsSolid(const int entity) const {
    return _tiles.Has(entity) || _bricks.Has(entity);
}

bool EnemyAISystem::IsPlayerInView(const int goombaEntity, const int playerEntity) const {
    const auto& goomba = _goombas.Get(goombaEntity);
    const auto& goombaPosition = _positions.Get(goombaEntity).Position;
    const auto& playerPosition = _positions.Get(playerEntity).Position;

    const float dx = playerPosition.x - goombaPosition.x;
    const float dy = playerPosition.y - goombaPosition.y;
    const float distance = dx * dx + dy * dy;
    const float viewDistance = goomba.ViewDistance * goomba.ViewDistance;

    return distance <= viewDistance && !HasObstacleBetween(goombaPosition, playerPosition);
}

bool EnemyAISystem::HasObstacleBetween(const sf::Vector2f& from, const sf::Vector2f& to) const {
    for (const int entity : _solidEntities) {
        if (!IsSolid(entity))
            continue;

        if (LineIntersectsBox(from, to, entity))
            return true;
    }

    return false;
}

bool EnemyAISystem::LineIntersectsBox(const sf::Vector2f& from,
    const sf::Vector2f& to,
    const int boxEntity) const {
    const auto& position = _positions.Get(boxEntity).Position;
    const auto& box = _boxColliders.Get(boxEntity);

    const float minX = position.x - box.Extents.x;
    const float maxX = position.x + box.Extents.x;
    const float minY = position.y - box.Extents.y;
    const float maxY = position.y + box.Extents.y;
    const float dx = to.x - from.x;
    const float dy = to.y - from.y;

    float minDistance = 0.0f;
    float maxDistance = 1.0f;

    if (dx == 0.0f) {
        if (from.x < minX || from.x > maxX)
            return false;
    } else {
        float firstDistance = (minX - from.x) / dx;
        float secondDistance = (maxX - from.x) / dx;
        if (firstDistance > secondDistance) {
            const float temp = firstDistance;
            firstDistance = secondDistance;
            secondDistance = temp;
        }
        if (firstDistance > minDistance)
            minDistance = firstDistance;
        if (secondDistance < maxDistance)
            maxDistance = secondDistance;
    }

    if (dy == 0.0f) {
        if (from.y < minY || from.y > maxY)
            return false;
    } else {
        float firstDistance = (minY - from.y) / dy;
        float secondDistance = (maxY - from.y) / dy;
        if (firstDistance > secondDistance) {
            const float temp = firstDistance;
            firstDistance = secondDistance;
            secondDistance = temp;
        }
        if (firstDistance > minDistance)
            minDistance = firstDistance;
        if (secondDistance < maxDistance)
            maxDistance = secondDistance;
    }

    return maxDistance >= minDistance;
}

void EnemyAISystem::OnUpdate() {
    for (const int goombaEntity : _goombaEntities) {
        auto& goomba = _goombas.Get(goombaEntity);
        goomba.IsChasing = false;

        for (const int playerEntity : _playerEntities) {
            if (!IsPlayerInView(goombaEntity, playerEntity))
                continue;

            goomba.IsChasing = true;
            break;
        }
    }
}
