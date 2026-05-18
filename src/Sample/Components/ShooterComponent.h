#ifndef SHOOTERCOMPONENT_H
#define SHOOTERCOMPONENT_H

struct ShooterComponent
{
    float CooldownMs;
    float TimeToShootMs = 0.0f;
    bool CanShoot = true;

    ShooterComponent() = default;
    ShooterComponent(const float cooldownMs) : CooldownMs(cooldownMs) {}
    void Shot()
    {
        CanShoot = false;
        TimeToShootMs = CooldownMs;
    }
    void Update(const float deltaTimeMs)
    {
        if (CanShoot)
            return;
        TimeToShootMs -= deltaTimeMs;
        if (TimeToShootMs <= 0.0f)
        {
            TimeToShootMs = 0.0f;
            CanShoot = true;
        }
    }
};

#endif //SHOOTERCOMPONENT_H
