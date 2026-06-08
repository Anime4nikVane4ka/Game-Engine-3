#include "RenderSystem.h"

#include <cmath>

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Angle.hpp>

void RenderSystem::OnInit()
{
}

void RenderSystem::DrawSprite(const int entity)
{
    const auto& position = _positions.Get(entity);
    auto& spriteComponent = _sprites.Get(entity);

    if (spriteComponent.Texture == nullptr)
        return;

    sf::Sprite sprite(*spriteComponent.Texture);
    sprite.setTextureRect(spriteComponent.TextureRect);
    sprite.setOrigin({
        spriteComponent.TextureRect.size.x / 2.0f,
        spriteComponent.TextureRect.size.y / 2.0f
    });
    sprite.setPosition(position.Position);

    sf::Vector2f scale = position.Scale;
    if (_players.Has(entity) && _movements.Has(entity))
    {
        const auto& movement = _movements.Get(entity);
        if (movement.Direction.x != 0.0f)
            spriteComponent.DirectionX = movement.Direction.x;

        scale.x = std::abs(scale.x) * spriteComponent.DirectionX;
    }
    sprite.setScale(scale);
    sprite.setRotation(sf::degrees(position.Angle));

    _window.draw(sprite);
}

void RenderSystem::OnUpdate()
{
    for (const int entity : _renderEntities)
        if (!_players.Has(entity))
            DrawSprite(entity);

    for (const int entity : _renderEntities)
        if (_players.Has(entity))
            DrawSprite(entity);
}
