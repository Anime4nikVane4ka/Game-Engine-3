#include "GravitySystem.h"

void GravitySystem::OnInit()
{
}

void GravitySystem::OnUpdate()
{
    for (const int entity : _entities)
    {
        const auto& gravity = _gravityComponents.Get(entity);
        auto& movement = _movementComponents.Get(entity);

        movement.Direction.y += gravity.Acceleration;
    }
}
