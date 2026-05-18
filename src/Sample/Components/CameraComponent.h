#ifndef CAMERACOMPONENT_H
#define CAMERACOMPONENT_H

#include <SFML/Graphics/View.hpp>

struct CameraComponent
{
    sf::View View;

    CameraComponent() = default;
    CameraComponent(const sf::View& view)
        : View(view) {}
};

#endif //CAMERACOMPONENT_H
