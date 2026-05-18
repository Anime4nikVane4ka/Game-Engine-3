#include "InputSystem.h"

void InputSystem::OnInit()
{
}

bool InputSystem::IsActive(const std::shared_ptr<InputAction>& action) const
{
    return action != nullptr && action->Type() == Start;
}

void InputSystem::OnUpdate()
{
    sf::Vector2f direction(0.0f, 0.0f);

    if (IsActive(_moveLeft))
        direction.x -= 1.0f;

    if (IsActive(_moveRight))
        direction.x += 1.0f;

    const bool jumpActive = IsActive(_jump);
    if (jumpActive && !_jumpWasActive)
        direction.y -= 1.0f;
    _jumpWasActive = jumpActive;

    for (const int player : _players)
    {
        auto& movement = _movementComponents.Get(player);
        movement.Direction.x = direction.x;

        if (direction.y != 0.0f)
            movement.Direction.y = direction.y;
    }
}
