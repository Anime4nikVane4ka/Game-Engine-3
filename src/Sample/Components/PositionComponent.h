#ifndef POSITIONCOMPONENT_H
#define POSITIONCOMPONENT_H

#include <SFML/System/Vector2.hpp>

struct PositionComponent
{
    sf::Vector2f Position = {0.0f, 0.0f};
    sf::Vector2f Scale = {1.0f, 1.0f};
    float Angle = 0.0f;

    PositionComponent() = default;
    PositionComponent(const float x, const float y): Position(x, y){}
};

#endif //POSITIONCOMPONENT_H
