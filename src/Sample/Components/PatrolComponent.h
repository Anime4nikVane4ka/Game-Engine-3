#ifndef PATROLCOMPONENT_H
#define PATROLCOMPONENT_H

#include <SFML/System/Vector2.hpp>

struct PatrolComponent {
    sf::Vector2f FirstPoint = {0.0f, 0.0f};
    sf::Vector2f SecondPoint = {0.0f, 0.0f};
    bool MovingToSecondPoint = true;

    PatrolComponent() = default;
    PatrolComponent(const sf::Vector2f& firstPoint, const sf::Vector2f& secondPoint)
        : FirstPoint(firstPoint), SecondPoint(secondPoint) {}
};

#endif // PATROLCOMPONENT_H
