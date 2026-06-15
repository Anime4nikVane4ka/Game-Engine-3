#ifndef HEALTHCOMPONENT_H
#define HEALTHCOMPONENT_H

struct HealthComponent {
    int Health = 1;
    int MaxHealth = 1;

    float InvulnerabilityTimeMs = 0.0f;
    float InvulnerabilityLeftMs = 0.0f;

    HealthComponent() = default;
    HealthComponent(const int health) : Health(health), MaxHealth(health) {}
    HealthComponent(const int health, const float invulnerabilityTimeMs) : Health(health), MaxHealth(health), InvulnerabilityTimeMs(invulnerabilityTimeMs) {}
};

#endif // HEALTHCOMPONENT_H
