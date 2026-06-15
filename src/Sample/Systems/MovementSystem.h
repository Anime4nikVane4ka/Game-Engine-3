#ifndef MOVEMENTSYSTEM_H
#define MOVEMENTSYSTEM_H

#include "../../Ecs/Filter/Filter.h"
#include "../../Ecs/Filter/FilterBuilder.h"
#include "../../Ecs/Systems/ISystem.h"
#include "../Components/MovementComponent.h"
#include "../Components/PositionComponent.h"

class MovementSystem final : public ISystem {
    ComponentStorage<PositionComponent>& _positionComponents;
    ComponentStorage<MovementComponent>& _movementComponents;

    Filter _moveables;

  public:
    MovementSystem(World& world)
        : ISystem(world), _positionComponents(world.GetStorage<PositionComponent>()), _movementComponents(world.GetStorage<MovementComponent>()),
          _moveables(FilterBuilder(world).With<PositionComponent>().With<MovementComponent>().Build()) {}

    void OnInit() override;
    void OnUpdate() override;
};

#endif // MOVEMENTSYSTEM_H
