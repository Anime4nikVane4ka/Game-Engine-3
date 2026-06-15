#include "MovementSystem.h"

namespace {
float Clamp(const float value, const float min, const float max) {
    if (value < min)
        return min;

    if (value > max)
        return max;

    return value;
}
} // namespace

void MovementSystem::OnInit() {}

void MovementSystem::OnUpdate() {
    for (const int entity : _moveables) {
        auto& position = _positionComponents.Get(entity);
        const auto& movement = _movementComponents.Get(entity);

        const float velocityX = Clamp(movement.Speed * movement.Direction.x, -movement.MaxVelocity, movement.MaxVelocity);
        const float velocityY = Clamp(movement.Speed * movement.Direction.y, -movement.MaxVelocity, movement.MaxVelocity);

        position.Position.x += velocityX;
        position.Position.y += velocityY;
    }
}
