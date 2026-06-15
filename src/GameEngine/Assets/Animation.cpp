#include "Animation.h"

Animation::Animation(const sf::Texture& texture, const short frameCount, const int frameDuration) : _texture(texture), _frameCount(frameCount), _frameDuration(frameDuration) {
    const auto textureSize = texture.getSize();
    const int frameWidth = frameCount <= 0 ? 0 : static_cast<int>(textureSize.x / frameCount);
    _size = {frameWidth, static_cast<int>(textureSize.y)};
}

Animation::Animation(const Animation& other) : _texture(other._texture), _frameCount(other._frameCount), _frameDuration(other._frameDuration), _size(other._size) {}

Animation::Animation(const Animation&& other) : _texture(other._texture), _frameCount(other._frameCount), _frameDuration(other._frameDuration), _size(other._size) {}
