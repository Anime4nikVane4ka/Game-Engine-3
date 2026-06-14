#include "RenderSystem.h"

#include <cmath>

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/System/Angle.hpp>

#include <imgui.h>
#include <string>

void RenderSystem::OnInit() {
    if (_renderMode != RenderMode::Textures) {
        return;
    }

    for (const int entity : _renderEntities) {
        if (!_players.Has(entity)) {
            DrawSprite(entity);
        }
    }

    for (const int entity : _renderEntities) {
        if (_players.Has(entity)) {
            DrawSprite(entity);
        }
    }
}

void RenderSystem::DrawColliders() {
    for (const int entity : _positionEntities) {
        const auto& position = _positions.Get(entity).Position;

        if (_boxColliders.Has(entity)) {
            const auto& box = _boxColliders.Get(entity);

            sf::RectangleShape rectangle(box.Size);
            rectangle.setOrigin(box.Extents);
            rectangle.setPosition(position);
            rectangle.setFillColor(sf::Color::Transparent);
            rectangle.setOutlineColor(sf::Color::Green);
            rectangle.setOutlineThickness(2.0f);

            _window.draw(rectangle);
        }

        if (_circleColliders.Has(entity)) {
            const auto& circle = _circleColliders.Get(entity);

            sf::CircleShape shape(circle.Radius);
            shape.setOrigin({circle.Radius, circle.Radius});
            shape.setPosition(position);
            shape.setFillColor(sf::Color::Transparent);
            shape.setOutlineColor(sf::Color::Red);
            shape.setOutlineThickness(2.0f);

            _window.draw(shape);
        }
    }
}

void RenderSystem::DrawGrid() {
    constexpr float cellSize = 64.0f;

    const sf::View view = _window.getView();
    const sf::Vector2f center = view.getCenter();
    const sf::Vector2f size = view.getSize();

    const float left = center.x - size.x / 2.0f;
    const float right = center.x + size.x / 2.0f;
    const float top = center.y - size.y / 2.0f;
    const float bottom = center.y + size.y / 2.0f;

    sf::VertexArray lines(sf::PrimitiveType::Lines);

    const float startX = std::floor(left / cellSize) * cellSize;
    const float startY = std::floor(top / cellSize) * cellSize;

    for (float x = startX; x <= right; x += cellSize) {
        lines.append(sf::Vertex({x, top}, sf::Color(255, 255, 255, 80)));
        lines.append(sf::Vertex({x, bottom}, sf::Color(255, 255, 255, 80)));
    }

    for (float y = startY; y <= bottom; y += cellSize) {
        lines.append(sf::Vertex({left, y}, sf::Color(255, 255, 255, 80)));
        lines.append(sf::Vertex({right, y}, sf::Color(255, 255, 255, 80)));
    }

    _window.draw(lines);

    ImDrawList* drawList = ImGui::GetForegroundDrawList();

    for (float x = startX; x <= right; x += cellSize) {
        for (float y = startY; y <= bottom; y += cellSize) {
            const int gridX = static_cast<int>(x / cellSize);
            const int gridY = _levelHeight - static_cast<int>(y / cellSize);

            if (gridY < 0 || gridY > _levelHeight) {
                continue;
            }

            const sf::Vector2i screenPosition = _window.mapCoordsToPixel({x + 4.0f, y - 18.0f});
            const std::string text = "(" + std::to_string(gridX) + ", " + std::to_string(gridY) + ")";

            drawList->AddText(ImVec2(static_cast<float>(screenPosition.x), static_cast<float>(screenPosition.y)), IM_COL32(255, 255, 255, 180), text.c_str());
        }
    }
}

void RenderSystem::DrawSprite(const int entity) {
    const auto& position = _positions.Get(entity);
    auto& spriteComponent = _sprites.Get(entity);

    if (spriteComponent.Texture == nullptr) {
        return;
    }

    sf::Sprite sprite(*spriteComponent.Texture);
    sprite.setTextureRect(spriteComponent.TextureRect);
    sprite.setOrigin({spriteComponent.TextureRect.size.x / 2.0f, spriteComponent.TextureRect.size.y / 2.0f});
    sprite.setPosition(position.Position);

    sf::Vector2f scale = position.Scale;
    if (_players.Has(entity) && _movements.Has(entity)) {
        const auto& movement = _movements.Get(entity);

        if (movement.Direction.x != 0.0f) {
            spriteComponent.DirectionX = movement.Direction.x;
        }

        scale.x = std::abs(scale.x) * spriteComponent.DirectionX;
    }
    sprite.setScale(scale);
    sprite.setRotation(sf::degrees(position.Angle));

    _window.draw(sprite);
}

void RenderSystem::OnUpdate() {
    if (_renderMode == RenderMode::Textures) {
        for (const int entity : _renderEntities) {
            if (!_players.Has(entity)) {
                DrawSprite(entity);
            }
        }

        for (const int entity : _renderEntities) {
            if (_players.Has(entity)) {
                DrawSprite(entity);
            }
        }

        return;
    }

    if (_renderMode == RenderMode::Colliders) {
        DrawColliders();
        return;
    }

    if (_renderMode == RenderMode::Grid) {
        for (const int entity : _renderEntities)
            DrawSprite(entity);

        DrawGrid();
    }
}
