#include "PatrolSystem.h"

#include <cmath>

void PatrolSystem::OnInit() {}

void PatrolSystem::OnUpdate() {
    for (const int entity : _patrolEntities) {
        auto& movement = _movements.Get(entity);

        if (_goombas.Has(entity) && _goombas.Get(entity).IsChasing) {
            movement.Direction.x = 0.0f;
            continue;
        }

        auto& patrol = _patrols.Get(entity);
        const auto& position = _positions.Get(entity);
        float targetX = patrol.MovingToSecondPoint ? patrol.SecondPoint.x : patrol.FirstPoint.x;
        float distance = targetX - position.Position.x;

        if (std::abs(distance) <= movement.Speed) {
            patrol.MovingToSecondPoint = !patrol.MovingToSecondPoint;
            targetX = patrol.MovingToSecondPoint ? patrol.SecondPoint.x : patrol.FirstPoint.x;
            distance = targetX - position.Position.x;
        }

        if (distance > 0.0f)
            movement.Direction.x = 1.0f;
        else if (distance < 0.0f)
            movement.Direction.x = -1.0f;
        else
            movement.Direction.x = 0.0f;
    }
}
