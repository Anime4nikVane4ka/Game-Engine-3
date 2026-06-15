#include "PatrolSystem.h"

#include <cmath>

void PatrolSystem::OnInit() {}

void PatrolSystem::OnUpdate() {
    const float deltaTimeMs = _clock.restart().asMilliseconds();

    for (const int entity : _patrolEntities) {
        auto& movement = _movements.Get(entity);
        auto& patrol = _patrols.Get(entity);
        const auto& position = _positions.Get(entity);

        if (_goombas.Has(entity)) {
            auto& goomba = _goombas.Get(entity);

            if (goomba.IsChasing)
                continue;

            if (goomba.WaitBeforePatrolMs > 0.0f) {
                movement.Direction.x = 0.0f;
                goomba.WaitBeforePatrolMs -= deltaTimeMs;

                if (goomba.WaitBeforePatrolMs <= 0.0f) {
                    goomba.WaitBeforePatrolMs = 0.0f;
                    goomba.IsReturningToPatrol = true;
                }

                continue;
            }

            if (goomba.IsReturningToPatrol) {
                const float firstDistance = std::abs(patrol.FirstPoint.x - position.Position.x);
                const float secondDistance = std::abs(patrol.SecondPoint.x - position.Position.x);
                const bool firstPointIsCloser = firstDistance <= secondDistance;
                const float targetX = firstPointIsCloser ? patrol.FirstPoint.x : patrol.SecondPoint.x;
                const float distance = targetX - position.Position.x;

                if (std::abs(distance) <= movement.Speed) {
                    movement.Direction.x = 0.0f;
                    goomba.IsReturningToPatrol = false;
                    patrol.MovingToSecondPoint = firstPointIsCloser;
                    continue;
                }

                if (distance > 0.0f)
                    movement.Direction.x = 1.0f;
                else
                    movement.Direction.x = -1.0f;

                continue;
            }
        }

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
