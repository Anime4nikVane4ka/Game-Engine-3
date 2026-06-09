#ifndef SPRITECOMPONENT_H
#define SPRITECOMPONENT_H

#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Texture.hpp>

struct SpriteComponent {
    const sf::Texture* Texture = nullptr;
    sf::IntRect TextureRect;
    float DirectionX = 1.0f;

    SpriteComponent() = default;
    SpriteComponent(const sf::Texture& texture)
        : Texture(&texture),
          TextureRect({0, 0},
              {static_cast<int>(texture.getSize().x), static_cast<int>(texture.getSize().y)}) {}
};

#endif // SPRITECOMPONENT_H
