#ifndef MOVEMENTCOMPONENT_H
#define MOVEMENTCOMPONENT_H

#include <SFML/System/Vector2.hpp>

struct MovementComponent
{
    float Speed = 0.0f;
    sf::Vector2f Direction = {0.0f, 0.0f};

    MovementComponent() = default;
    MovementComponent(const float speed, const sf::Vector2f& direction): Speed(speed), Direction(direction) {}
    MovementComponent(const float speed, const float x, const float y): Speed(speed), Direction(x, y) {}
};

#endif //MOVEMENTCOMPONENT_H
