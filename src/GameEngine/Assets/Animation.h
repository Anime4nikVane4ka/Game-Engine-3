#ifndef ANIMATION_H
#define ANIMATION_H

#include "SFML/Graphics/Texture.hpp"

class Animation
{
    const sf::Texture& _texture;
    short _frameCount;
    int _frameDuration;

    sf::Vector2i _size;

public:
    // ToDo:
    Animation(const sf::Texture& texture, short frameCount, int frameDuration);

    // ToDo:
    Animation(const Animation& other);

    // ToDo:
    Animation(const Animation&& other);

    const sf::Texture& GetTexture() const { return _texture; }
    short FrameCount() const { return _frameCount; }
    int FrameDuration() const { return _frameDuration; }
    const sf::Vector2i& Size() const { return _size; }
};

#endif //ANIMATION_H
