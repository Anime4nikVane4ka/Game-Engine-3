#ifndef HEALTHSYSTEM_H
#define HEALTHSYSTEM_H

#include <vector>

#include <SFML/System/Clock.hpp>
#include <SFML/System/Time.hpp>

#include "../../Ecs/Filter/Filter.h"
#include "../../Ecs/Filter/FilterBuilder.h"
#include "../../Ecs/Systems/ISystem.h"
#include "../Components/BulletComponent.h"
#include "../Components/DamageEventComponent.h"
#include "../Components/GoombaComponent.h"
#include "../Components/HealthComponent.h"
#include "../Components/PlayerComponent.h"
#include "../Components/RespawnComponent.h"

class HealthSystem final : public ISystem {
    ComponentStorage<BulletComponent>& _bullets;
    ComponentStorage<DamageEventComponent>& _damageEvents;
    ComponentStorage<GoombaComponent>& _goombas;
    ComponentStorage<HealthComponent>& _healths;
    ComponentStorage<PlayerComponent>& _players;
    ComponentStorage<RespawnComponent>& _respawns;

    Filter _healthEntities;
    Filter _damageEventEntities;
    sf::Clock _clock;

    bool ApplyDamage(int entity, int damage);
    void HandlePlayerDamage(int playerEntity);
    void HandleGoombaDamage(int goombaEntity, int bulletEntity, std::vector<int>& entitiesToRemove);
    void HandleDamageEvent(int eventEntity, std::vector<int>& entitiesToRemove);

  public:
    HealthSystem(World& world)
        : ISystem(world), _bullets(world.GetStorage<BulletComponent>()), _damageEvents(world.GetStorage<DamageEventComponent>()), _goombas(world.GetStorage<GoombaComponent>()),
          _healths(world.GetStorage<HealthComponent>()), _players(world.GetStorage<PlayerComponent>()), _respawns(world.GetStorage<RespawnComponent>()),
          _healthEntities(FilterBuilder(world).With<HealthComponent>().Build()), _damageEventEntities(FilterBuilder(world).With<DamageEventComponent>().Build()) {}

    void OnInit() override;
    void OnUpdate() override;
};

#endif // HEALTHSYSTEM_H
