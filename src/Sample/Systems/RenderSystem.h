#ifndef RENDERSYSTEM_H
#define RENDERSYSTEM_H

#include <SFML/Graphics/RenderWindow.hpp>

#include "../../Ecs/Filter/Filter.h"
#include "../../Ecs/Filter/FilterBuilder.h"
#include "../../Ecs/Systems/ISystem.h"
#include "../Components/BoxColliderComponent.h"
#include "../Components/CircleColliderComponent.h"
#include "../Components/HealthComponent.h"
#include "../Components/MovementComponent.h"
#include "../Components/PlayerComponent.h"
#include "../Components/PositionComponent.h"
#include "../Components/SpriteComponent.h"

enum class RenderMode { Textures, Colliders, Grid };

class RenderSystem final : public ISystem {
    sf::RenderWindow& _window;
    ComponentStorage<MovementComponent>& _movements;
    ComponentStorage<PlayerComponent>& _players;
    ComponentStorage<PositionComponent>& _positions;
    ComponentStorage<SpriteComponent>& _sprites;
    ComponentStorage<HealthComponent>& _healths;
    ComponentStorage<BoxColliderComponent>& _boxColliders;
    ComponentStorage<CircleColliderComponent>& _circleColliders;

    RenderMode& _renderMode;

    Filter _positionEntities;

    Filter _renderEntities;

    int _levelHeight;

    void DrawSprite(int entity);
    void DrawHealthBars();

  public:
    RenderSystem(World& world, sf::RenderWindow& window, RenderMode& renderMode, int levelHeight)
        : ISystem(world), _window(window), _movements(world.GetStorage<MovementComponent>()), _players(world.GetStorage<PlayerComponent>()), _positions(world.GetStorage<PositionComponent>()),
          _sprites(world.GetStorage<SpriteComponent>()), _healths(world.GetStorage<HealthComponent>()), _boxColliders(world.GetStorage<BoxColliderComponent>()),
          _circleColliders(world.GetStorage<CircleColliderComponent>()), _renderMode(renderMode), _levelHeight(levelHeight),
          _renderEntities(FilterBuilder(world).With<PositionComponent>().With<SpriteComponent>().Build()), _positionEntities(FilterBuilder(world).With<PositionComponent>().Build()) {}
    void DrawColliders();
    void DrawGrid();
    void OnInit() override;
    void OnUpdate() override;
};

#endif // RENDERSYSTEM_H
