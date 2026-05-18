#ifndef CIRCLECOLLIDERCOMPONENT_H
#define CIRCLECOLLIDERCOMPONENT_H

struct CircleColliderComponent
{
    float Radius;

    CircleColliderComponent() = default;
    CircleColliderComponent(const float radius): Radius(radius) {}
};

#endif //CIRCLECOLLIDERCOMPONENT_H
