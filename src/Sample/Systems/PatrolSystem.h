#ifndef PATROLSYSTEM_H
#define PATROLSYSTEM_H

#include <SFML/System/Clock.hpp>
#include <SFML/System/Time.hpp>

#include "../../Ecs/Filter/Filter.h"
#include "../../Ecs/Filter/FilterBuilder.h"
#include "../../Ecs/Systems/ISystem.h"
#include "../Components/GoombaComponent.h"
#include "../Components/MovementComponent.h"
#include "../Components/PatrolComponent.h"
#include "../Components/PositionComponent.h"

class PatrolSystem final : public ISystem {
    ComponentStorage<GoombaComponent>& _goombas;
    ComponentStorage<MovementComponent>& _movements;
    ComponentStorage<PatrolComponent>& _patrols;
    ComponentStorage<PositionComponent>& _positions;

    Filter _patrolEntities;
    sf::Clock _clock;

  public:
    PatrolSystem(World& world)
        : ISystem(world), _goombas(world.GetStorage<GoombaComponent>()), _movements(world.GetStorage<MovementComponent>()), _patrols(world.GetStorage<PatrolComponent>()),
          _positions(world.GetStorage<PositionComponent>()), _patrolEntities(FilterBuilder(world).With<MovementComponent>().With<PatrolComponent>().With<PositionComponent>().Build()) {}

    void OnInit() override;
    void OnUpdate() override;
};

#endif // PATROLSYSTEM_H
