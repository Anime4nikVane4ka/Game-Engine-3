#ifndef DESTROYONANIMATIONENDSYSTEM_H
#define DESTROYONANIMATIONENDSYSTEM_H

#include "../../Ecs/Filter/Filter.h"
#include "../../Ecs/Filter/FilterBuilder.h"
#include "../../Ecs/Systems/ISystem.h"
#include "../Components/AnimatorComponent.h"
#include "../Components/DestroyOnAnimationEndComponent.h"

class DestroyOnAnimationEndSystem final : public ISystem
{
    ComponentStorage<AnimatorComponent>& _animators;

    Filter _entities;

public:
    DestroyOnAnimationEndSystem(World& world)
        : ISystem(world),
          _animators(world.GetStorage<AnimatorComponent>()),
          _entities(FilterBuilder(world)
              .With<AnimatorComponent>()
              .With<DestroyOnAnimationEndComponent>()
              .Build())
    {
    }

    void OnInit() override;
    void OnUpdate() override;
};

#endif //DESTROYONANIMATIONENDSYSTEM_H
