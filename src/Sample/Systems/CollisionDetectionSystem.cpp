#include "CollisionDetectionSystem.h"

#include <algorithm>
#include <cmath>
#include <vector>

void CollisionDetectionSystem::OnInit()
{
}

bool CollisionDetectionSystem::CheckAabbVsAabb(const int firstEntity, const int secondEntity)
{
    const auto& firstPosition = _positions.Get(firstEntity).Position;
    const auto& secondPosition = _positions.Get(secondEntity).Position;
    const auto& firstBox = _boxColliders.Get(firstEntity);
    const auto& secondBox = _boxColliders.Get(secondEntity);

    return std::abs(firstPosition.x - secondPosition.x) <= firstBox.Extents.x + secondBox.Extents.x
        && std::abs(firstPosition.y - secondPosition.y) <= firstBox.Extents.y + secondBox.Extents.y;
}

bool CollisionDetectionSystem::CheckAabbVsCircle(const int boxEntity, const int circleEntity)
{
    const auto& boxPosition = _positions.Get(boxEntity).Position;
    const auto& circlePosition = _positions.Get(circleEntity).Position;
    const auto& box = _boxColliders.Get(boxEntity);
    const auto& circle = _circleColliders.Get(circleEntity);

    const float closestX = std::clamp(
        circlePosition.x,
        boxPosition.x - box.Extents.x,
        boxPosition.x + box.Extents.x);
    const float closestY = std::clamp(
        circlePosition.y,
        boxPosition.y - box.Extents.y,
        boxPosition.y + box.Extents.y);

    const float dx = circlePosition.x - closestX;
    const float dy = circlePosition.y - closestY;

    return dx * dx + dy * dy <= circle.Radius * circle.Radius;
}

bool CollisionDetectionSystem::CheckCircleVsCircle(const int firstEntity, const int secondEntity)
{
    const auto& firstPosition = _positions.Get(firstEntity).Position;
    const auto& secondPosition = _positions.Get(secondEntity).Position;
    const float firstRadius = _circleColliders.Get(firstEntity).Radius;
    const float secondRadius = _circleColliders.Get(secondEntity).Radius;

    const float dx = firstPosition.x - secondPosition.x;
    const float dy = firstPosition.y - secondPosition.y;
    const float radiusSum = firstRadius + secondRadius;

    return dx * dx + dy * dy <= radiusSum * radiusSum;
}

bool CollisionDetectionSystem::CheckCollision(const int firstEntity, const int secondEntity)
{
    const bool firstHasBox = _boxColliders.Has(firstEntity);
    const bool secondHasBox = _boxColliders.Has(secondEntity);
    const bool firstHasCircle = _circleColliders.Has(firstEntity);
    const bool secondHasCircle = _circleColliders.Has(secondEntity);

    if (firstHasBox && secondHasBox && CheckAabbVsAabb(firstEntity, secondEntity))
        return true;

    if (firstHasBox && secondHasCircle && CheckAabbVsCircle(firstEntity, secondEntity))
        return true;

    if (firstHasCircle && secondHasBox && CheckAabbVsCircle(secondEntity, firstEntity))
        return true;

    if (firstHasCircle && secondHasCircle && CheckCircleVsCircle(firstEntity, secondEntity))
        return true;

    return false;
}

void CollisionDetectionSystem::OnUpdate()
{
    std::vector<int> entities;
    for (const auto entity : _collidableEntities)
    {
        _collisions.Get(entity).Clear();
        entities.push_back(entity);
    }

    for (int i = 0; i < entities.size(); i++)
    {
        for (int j = i + 1; j < entities.size(); j++)
        {
            const int firstEntity = entities[i];
            const int secondEntity = entities[j];

            if (!CheckCollision(firstEntity, secondEntity))
                continue;

            _collisions.Get(firstEntity).Add(secondEntity);
            _collisions.Get(secondEntity).Add(firstEntity);
        }
    }
}
