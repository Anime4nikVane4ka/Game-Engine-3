#ifndef GRAVITYSYSTEM_H
#define GRAVITYSYSTEM_H

#include "../../Ecs/Filter/Filter.h"
#include "../../Ecs/Filter/FilterBuilder.h"
#include "../../Ecs/Systems/ISystem.h"
#include "../Components/GravityComponent.h"
#include "../Components/MovementComponent.h"

class GravitySystem final : public ISystem
{
    ComponentStorage<GravityComponent>& _gravityComponents;
    ComponentStorage<MovementComponent>& _movementComponents;

    Filter _entities;

public:
    GravitySystem(World& world)
        : ISystem(world),
          _gravityComponents(world.GetStorage<GravityComponent>()),
          _movementComponents(world.GetStorage<MovementComponent>()),
          _entities(FilterBuilder(world)
              .With<GravityComponent>()
              .With<MovementComponent>()
              .Build())
    {
    }

    void OnInit() override;
    void OnUpdate() override;
};

#endif //GRAVITYSYSTEM_H
