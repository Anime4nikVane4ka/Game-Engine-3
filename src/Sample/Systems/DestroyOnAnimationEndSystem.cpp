#include "DestroyOnAnimationEndSystem.h"

#include <vector>

void DestroyOnAnimationEndSystem::OnInit() {}

void DestroyOnAnimationEndSystem::OnUpdate() {
    std::vector<int> entitiesToRemove;

    for (const int entity : _entities) {
        auto& destroyOnAnimationEnd = _destroyOnAnimationEnds.Get(entity);

        if (destroyOnAnimationEnd.FramesRemaining == 0 || (destroyOnAnimationEnd.FramesRemaining < 0 && _animators.Get(entity).AnimationFinished)) {
            entitiesToRemove.push_back(entity);
            continue;
        }

        if (destroyOnAnimationEnd.FramesRemaining > 0) {
            --destroyOnAnimationEnd.FramesRemaining;
        }
    }

    for (const int entity : entitiesToRemove) {
        if (world.IsEntityAlive(entity))
            world.RemoveEntity(entity);
    }
}
