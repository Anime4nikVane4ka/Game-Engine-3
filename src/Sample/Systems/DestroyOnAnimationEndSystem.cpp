#include "DestroyOnAnimationEndSystem.h"

#include <vector>

void DestroyOnAnimationEndSystem::OnInit() {}

void DestroyOnAnimationEndSystem::OnUpdate() {
    std::vector<int> entitiesToRemove;

    for (const int entity : _entities) {
        if (_animators.Get(entity).AnimationFinished)
            entitiesToRemove.push_back(entity);
    }

    for (const int entity : entitiesToRemove) {
        if (world.IsEntityAlive(entity))
            world.RemoveEntity(entity);
    }
}
