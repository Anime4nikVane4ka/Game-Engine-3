#ifndef RECTANGLESHAPECOMPONENT_H
#define RECTANGLESHAPECOMPONENT_H

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/System/Vector2.hpp>

struct RectangleShapeComponent
{
    sf::RectangleShape Shape;
    sf::Vector2f Size;
    sf::Color FillColor;

    RectangleShapeComponent() = default;
    RectangleShapeComponent(const sf::Vector2f& size, const sf::Color& fillColor = sf::Color::White)
        : Shape(size), Size(size), FillColor(fillColor)
    {
        Shape.setFillColor(FillColor);
        Shape.setOrigin({Size.x / 2.0f, Size.y / 2.0f});
    }
    RectangleShapeComponent(const float width, const float height, const sf::Color& fillColor = sf::Color::White) :
        RectangleShapeComponent({width, height}, fillColor)
    {
    }
};

#endif //RECTANGLESHAPECOMPONENT_H
