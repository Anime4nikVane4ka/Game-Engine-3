#ifndef SHOOTERCOMPONENT_H
#define SHOOTERCOMPONENT_H

struct ShooterComponent {
    float CooldownMs = 0.0f;
    float TimeToShootMs = 0.0f;
    float BulletSpeed = 0.0f;
    bool CanShoot = true;
    bool Shoot = false;
    float DirectionX = 1.0f;

    ShooterComponent() = default;
    ShooterComponent(const float cooldownMs) : CooldownMs(cooldownMs) {}
    ShooterComponent(const float cooldownMs, const float bulletSpeed) : CooldownMs(cooldownMs), BulletSpeed(bulletSpeed) {}
    void Shot() {
        CanShoot = false;
        TimeToShootMs = CooldownMs;
    }
    void Update(const float deltaTimeMs) {
        if (CanShoot)
            return;
        TimeToShootMs -= deltaTimeMs;
        if (TimeToShootMs <= 0.0f) {
            TimeToShootMs = 0.0f;
            CanShoot = true;
        }
    }
};

#endif // SHOOTERCOMPONENT_H
