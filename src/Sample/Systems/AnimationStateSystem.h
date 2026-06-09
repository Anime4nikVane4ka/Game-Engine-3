#ifndef ANIMATIONSTATESYSTEM_H
#define ANIMATIONSTATESYSTEM_H

#include "../../Ecs/Filter/Filter.h"
#include "../../Ecs/Filter/FilterBuilder.h"
#include "../../Ecs/Systems/ISystem.h"
#include "../Components/AnimationStateComponent.h"
#include "../Components/AnimatorComponent.h"
#include "../Components/MovementComponent.h"
#include "../Components/PlayerComponent.h"
#include "../Components/ShooterComponent.h"

class AnimationStateSystem final : public ISystem {
    ComponentStorage<AnimationStateComponent>& _animationStates;
    ComponentStorage<AnimatorComponent>& _animators;
    ComponentStorage<MovementComponent>& _movements;
    ComponentStorage<ShooterComponent>& _shooters;

    Filter _players;

  public:
    AnimationStateSystem(World& world)
        : ISystem(world), _animationStates(world.GetStorage<AnimationStateComponent>()),
          _animators(world.GetStorage<AnimatorComponent>()),
          _movements(world.GetStorage<MovementComponent>()),
          _shooters(world.GetStorage<ShooterComponent>()), _players(FilterBuilder(world)
                                                                   .With<PlayerComponent>()
                                                                   .With<AnimationStateComponent>()
                                                                   .With<AnimatorComponent>()
                                                                   .With<MovementComponent>()
                                                                   .Build()) {}

    void OnInit() override;
    void OnUpdate() override;
};

#endif // ANIMATIONSTATESYSTEM_H
