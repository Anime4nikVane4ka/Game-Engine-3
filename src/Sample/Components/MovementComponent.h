#ifndef MOVEMENTCOMPONENT_H
#define MOVEMENTCOMPONENT_H

#include <SFML/System/Vector2.hpp>

struct MovementComponent
{
    float Speed = 0.0f;
    float MaxVelocity = 2000.0f;
    sf::Vector2f Direction = {0.0f, 0.0f};

    MovementComponent() = default;
    MovementComponent(const float speed, const sf::Vector2f& direction): Speed(speed), Direction(direction) {}
    MovementComponent(const float speed, const float x, const float y): Speed(speed), Direction(x, y) {}
    MovementComponent(const float speed, const sf::Vector2f& direction, const float maxVelocity)
        : Speed(speed), MaxVelocity(maxVelocity), Direction(direction)
    {
    }
};

#endif //MOVEMENTCOMPONENT_H
