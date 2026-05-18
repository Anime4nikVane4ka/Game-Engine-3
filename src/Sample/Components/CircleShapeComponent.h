#ifndef CIRCLESHAPECOMPONENT_H
#define CIRCLESHAPECOMPONENT_H

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Color.hpp>

struct CircleShapeComponent
{
    sf::CircleShape Shape;
    float Radius;
    std::size_t PointCount;
    sf::Color FillColor;

    CircleShapeComponent() = default;
    CircleShapeComponent(const float radius = 5.0f, const std::size_t pointCount = 30, const sf::Color& fillColor = sf::Color::White)
        : Shape(radius, pointCount), Radius(radius), PointCount(pointCount), FillColor(fillColor)
    {
        Shape.setFillColor(FillColor);
        Shape.setOrigin({Radius, Radius}); // Чтобы рисовался из центра круга, а не из верхнего левого угла
    }
};

#endif //CIRCLESHAPECOMPONENT_H
