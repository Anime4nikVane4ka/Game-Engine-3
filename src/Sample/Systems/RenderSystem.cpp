#include "RenderSystem.h"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Angle.hpp>

void RenderSystem::OnInit()
{
}

void RenderSystem::OnUpdate()
{
    for (const int entity : _renderEntities)
    {
        const auto& position = _positions.Get(entity);
        const auto& spriteComponent = _sprites.Get(entity);

        if (spriteComponent.Texture == nullptr)
            continue;

        sf::Sprite sprite(*spriteComponent.Texture);
        sprite.setPosition(position.Position);
        sprite.setScale(position.Scale);
        sprite.setRotation(sf::degrees(position.Angle));

        if (spriteComponent.TextureRect.size.x > 0 && spriteComponent.TextureRect.size.y > 0)
            sprite.setTextureRect(spriteComponent.TextureRect);

        _window.draw(sprite);
    }
}
