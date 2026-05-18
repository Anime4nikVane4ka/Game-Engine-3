#ifndef GRAVITYCOMPONENT_H
#define GRAVITYCOMPONENT_H

struct GravityComponent
{
    float Acceleration = 980.0f;

    GravityComponent() = default;
    GravityComponent(const float acceleration)
        : Acceleration(acceleration)
    {
    }
};

#endif //GRAVITYCOMPONENT_H
