#ifndef RESPAWNSYSTEM_H
#define RESPAWNSYSTEM_H

#include "../../Ecs/Filter/Filter.h"
#include "../../Ecs/Filter/FilterBuilder.h"
#include "../../Ecs/Systems/ISystem.h"
#include "../Components/MovementComponent.h"
#include "../Components/PositionComponent.h"
#include "../Components/RespawnComponent.h"

class RespawnSystem final : public ISystem {
    ComponentStorage<PositionComponent>& _positions;
    ComponentStorage<MovementComponent>& _movements;
    ComponentStorage<RespawnComponent>& _respawns;

    Filter _entities;

    float _fallLimitY;

public:
    RespawnSystem(World& world, float fallLimitY)
        : ISystem(world),
          _positions(world.GetStorage<PositionComponent>()),
          _movements(world.GetStorage<MovementComponent>()),
          _respawns(world.GetStorage<RespawnComponent>()),
          _entities(FilterBuilder(world)
              .With<PositionComponent>()
              .With<MovementComponent>()
              .With<RespawnComponent>()
              .Build()),
          _fallLimitY(fallLimitY) {}

    void OnInit() override {}
    void OnUpdate() override;
};

#endif // RESPAWNSYSTEM_H