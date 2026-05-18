#ifndef BOXCOLLIDERCOMPONENT_H
#define BOXCOLLIDERCOMPONENT_H

#include <SFML/System/Vector2.hpp>

struct BoxColliderComponent
{
    sf::Vector2f Size;
    sf::Vector2f Extents;

    BoxColliderComponent() = default;
    BoxColliderComponent(const sf::Vector2f& size): Size(size), Extents(size.x / 2.0f, size.y / 2.0f) {}
    BoxColliderComponent(const float width, const float height): Size(width, height), Extents(width / 2.0f, height / 2.0f) {}
};

#endif //BOXCOLLIDERCOMPONENT_H
