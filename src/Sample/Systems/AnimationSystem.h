#ifndef ANIMATIONSYSTEM_H
#define ANIMATIONSYSTEM_H

#include "../../Ecs/Filter/Filter.h"
#include "../../Ecs/Filter/FilterBuilder.h"
#include "../../Ecs/Systems/ISystem.h"
#include "../Components/AnimationStateComponent.h"
#include "../Components/AnimatorComponent.h"
#include "../Components/SpriteComponent.h"

class AnimationSystem final : public ISystem
{
    ComponentStorage<AnimationStateComponent>& _animationStates;
    ComponentStorage<AnimatorComponent>& _animators;
    ComponentStorage<SpriteComponent>& _sprites;

    Filter _animatedEntities;

public:
    AnimationSystem(World& world)
        : ISystem(world),
          _animationStates(world.GetStorage<AnimationStateComponent>()),
          _animators(world.GetStorage<AnimatorComponent>()),
          _sprites(world.GetStorage<SpriteComponent>()),
          _animatedEntities(FilterBuilder(world)
              .With<AnimationStateComponent>()
              .With<AnimatorComponent>()
              .With<SpriteComponent>()
              .Build())
    {
    }

    void OnInit() override;
    void OnUpdate() override;
};

#endif //ANIMATIONSYSTEM_H
