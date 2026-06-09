#ifndef BULLETCOMPONENT_H
#define BULLETCOMPONENT_H

struct BulletComponent
{
    int Owner = -1;
    float Speed = 0.0f;

    BulletComponent() = default;
    BulletComponent(const int owner, const float speed): Owner(owner), Speed(speed) {}
};

#endif //BULLETCOMPONENT_H
