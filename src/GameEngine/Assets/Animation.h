#ifndef ANIMATION_H
#define ANIMATION_H

#include <cstddef>

#include "SFML/Graphics/Texture.hpp"

class Animation
{
    const sf::Texture& _texture;
    size_t _frameCount;
    size_t _frameDuration;

    sf::Vector2i _size;

public:
    // ToDo:
    Animation(const sf::Texture& texture, size_t frameCount, size_t frameDuration);

    // ToDo:
    Animation(const Animation& other);

    // ToDo:
    Animation(const Animation&& other);

    const sf::Texture& GetTexture() const { return _texture; }
    size_t FrameCount() const { return _frameCount; }
    size_t FrameDuration() const { return _frameDuration; }
    const sf::Vector2i& Size() const { return _size; }
};

#endif //ANIMATION_H
