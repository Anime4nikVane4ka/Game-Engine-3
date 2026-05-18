#ifndef COLLISIONCOMPONENT_H
#define COLLISIONCOMPONENT_H

#include <vector>

struct CollisionComponent
{
    std::vector<int> CollidedEntities;

    CollisionComponent() = default;
    void Add(const int collidedEntity) { CollidedEntities.push_back(collidedEntity); }
    void Clear() { CollidedEntities.clear(); }
};

#endif //COLLISIONCOMPONENT_H
