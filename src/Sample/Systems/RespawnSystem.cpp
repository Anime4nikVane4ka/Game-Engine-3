#include "RespawnSystem.h"

void RespawnSystem::OnUpdate() {
    for (const int entity : _entities) {
        auto& position = _positions.Get(entity);
        auto& respawn = _respawns.Get(entity);

        if (position.Position.y <= _fallLimitY && !respawn.NeedRespawn) {
            continue;
        }

        auto& movement = _movements.Get(entity);

        position.Position = respawn.Position;
        respawn.NeedRespawn = false;
        movement.Direction = {0.0f, 0.0f};
        movement.IsGrounded = false;
    }
}
