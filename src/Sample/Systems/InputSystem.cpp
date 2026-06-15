#include "InputSystem.h"

void InputSystem::OnInit() {}

bool InputSystem::IsActive(const std::shared_ptr<InputAction>& action) const {
    return action != nullptr && action->Type() == Start;
}

void InputSystem::OnUpdate() {
    sf::Vector2f direction(0.0f, 0.0f);

    if (IsActive(_moveLeft))
        direction.x -= 1.0f;

    if (IsActive(_moveRight))
        direction.x += 1.0f;

    const bool jumpActive = IsActive(_jump);
    const bool shootActive = IsActive(_shoot);

    for (const int player : _players) {
        auto& movement = _movementComponents.Get(player);
        movement.Direction.x = direction.x;

        if (jumpActive && !_jumpWasActive && movement.IsGrounded) {
            movement.Direction.y = -movement.JumpForce;
            movement.IsGrounded = false;
        }

        if (_shooterComponents.Has(player))
            _shooterComponents.Get(player).Shoot = shootActive;
    }

    _jumpWasActive = jumpActive;
}
