#ifndef BULLETCOMPONENT_H
#define BULLETCOMPONENT_H

struct BulletComponent
{
    int Owner = -1;

    BulletComponent() = default;
    BulletComponent(const int owner): Owner(owner) {}
};

#endif //BULLETCOMPONENT_H
