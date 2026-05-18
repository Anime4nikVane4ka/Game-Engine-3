#ifndef SPRITECOMPONENT_H
#define SPRITECOMPONENT_H

#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Texture.hpp>

struct SpriteComponent
{
    const sf::Texture* Texture = nullptr;
    sf::IntRect TextureRect;

    SpriteComponent() = default;
    SpriteComponent(const sf::Texture& texture)
        : Texture(&texture)
    {}
};

#endif //SPRITECOMPONENT_H
