#include "RespawnSystem.h"

void RespawnSystem::OnUpdate() {
    for (const int entity : _entities) {
        auto& position = _positions.Get(entity);

        if (position.Position.y <= _fallLimitY) {
            continue;
        }

        const auto& respawn = _respawns.Get(entity);
        auto& movement = _movements.Get(entity);

        position.Position = respawn.Position;
        movement.Direction = {0.0f, 0.0f};
        movement.IsGrounded = false;
    }
}