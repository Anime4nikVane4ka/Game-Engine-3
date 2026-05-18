#ifndef ANIMATIONSTATESYSTEM_H
#define ANIMATIONSTATESYSTEM_H

#include "../../Ecs/Filter/Filter.h"
#include "../../Ecs/Filter/FilterBuilder.h"
#include "../../Ecs/Systems/ISystem.h"
#include "../Components/AnimationStateComponent.h"
#include "../Components/MovementComponent.h"
#include "../Components/PlayerComponent.h"

class AnimationStateSystem final : public ISystem
{
    ComponentStorage<AnimationStateComponent>& _animationStates;
    ComponentStorage<MovementComponent>& _movements;

    Filter _players;

public:
    AnimationStateSystem(World& world)
        : ISystem(world),
          _animationStates(world.GetStorage<AnimationStateComponent>()),
          _movements(world.GetStorage<MovementComponent>()),
          _players(FilterBuilder(world)
              .With<PlayerComponent>()
              .With<AnimationStateComponent>()
              .With<MovementComponent>()
              .Build())
    {
    }

    void OnInit() override;
    void OnUpdate() override;
};

#endif //ANIMATIONSTATESYSTEM_H
