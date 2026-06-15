#include "HealthSystem.h"

#include <algorithm>

void HealthSystem::OnInit() {}

bool HealthSystem::ApplyDamage(const int entity, const int damage) {
    if (!_healths.Has(entity))
        return false;

    auto& health = _healths.Get(entity);
    if (health.InvulnerabilityLeftMs > 0.0f)
        return false;

    health.Health -= damage;
    health.InvulnerabilityLeftMs = health.InvulnerabilityTimeMs;
    return true;
}

void HealthSystem::HandlePlayerDamage(const int playerEntity) {
    if (!ApplyDamage(playerEntity, 1))
        return;

    auto& health = _healths.Get(playerEntity);
    if (health.Health > 0)
        return;

    health.Health = health.MaxHealth;
    if (_respawns.Has(playerEntity))
        _respawns.Get(playerEntity).NeedRespawn = true;
}

void HealthSystem::HandleGoombaDamage(const int goombaEntity,
    const int bulletEntity,
    std::vector<int>& entitiesToRemove) {
    if (std::find(entitiesToRemove.begin(), entitiesToRemove.end(), bulletEntity) ==
        entitiesToRemove.end())
        entitiesToRemove.push_back(bulletEntity);

    if (!ApplyDamage(goombaEntity, 1))
        return;

    if (_healths.Get(goombaEntity).Health <= 0 &&
        std::find(entitiesToRemove.begin(), entitiesToRemove.end(), goombaEntity) ==
            entitiesToRemove.end())
        entitiesToRemove.push_back(goombaEntity);
}

void HealthSystem::HandleDamageEvent(const int eventEntity,
    std::vector<int>& entitiesToRemove) {
    const auto& damageEvent = _damageEvents.Get(eventEntity);
    const int firstEntity = damageEvent.FirstEntity;
    const int secondEntity = damageEvent.SecondEntity;

    if (!world.IsEntityAlive(firstEntity) || !world.IsEntityAlive(secondEntity))
        return;

    if (_players.Has(firstEntity) && _goombas.Has(secondEntity)) {
        HandlePlayerDamage(firstEntity);
        return;
    }

    if (_players.Has(secondEntity) && _goombas.Has(firstEntity)) {
        HandlePlayerDamage(secondEntity);
        return;
    }

    if (_bullets.Has(firstEntity) && _goombas.Has(secondEntity)) {
        HandleGoombaDamage(secondEntity, firstEntity, entitiesToRemove);
        return;
    }

    if (_bullets.Has(secondEntity) && _goombas.Has(firstEntity))
        HandleGoombaDamage(firstEntity, secondEntity, entitiesToRemove);
}

void HealthSystem::OnUpdate() {
    const float deltaTimeMs = _clock.restart().asMilliseconds();
    std::vector<int> entitiesToRemove;
    std::vector<int> eventsToRemove;

    for (const int entity : _healthEntities) {
        auto& health = _healths.Get(entity);
        if (health.InvulnerabilityLeftMs > 0.0f) {
            health.InvulnerabilityLeftMs -= deltaTimeMs;
            if (health.InvulnerabilityLeftMs < 0.0f)
                health.InvulnerabilityLeftMs = 0.0f;
        }
    }

    for (const int eventEntity : _damageEventEntities) {
        HandleDamageEvent(eventEntity, entitiesToRemove);
        eventsToRemove.push_back(eventEntity);
    }

    for (const int entity : entitiesToRemove) {
        if (world.IsEntityAlive(entity))
            world.RemoveEntity(entity);
    }

    for (const int eventEntity : eventsToRemove) {
        if (world.IsEntityAlive(eventEntity))
            world.RemoveEntity(eventEntity);
    }
}
