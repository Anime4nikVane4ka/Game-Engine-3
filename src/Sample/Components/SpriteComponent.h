#ifndef SPRITECOMPONENT_H
#define SPRITECOMPONENT_H

#include <SFML/Graphics/Texture.hpp>

struct SpriteComponent
{
    const sf::Texture* Texture = nullptr;

    SpriteComponent() = default;
    explicit SpriteComponent(const sf::Texture& texture)
        : Texture(&texture)
    {}
};

#endif //SPRITECOMPONENT_H
