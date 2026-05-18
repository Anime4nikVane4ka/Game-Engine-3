#ifndef RENDERSYSTEM_H
#define RENDERSYSTEM_H

#include <SFML/Graphics/RenderWindow.hpp>

#include "../../Ecs/Filter/Filter.h"
#include "../../Ecs/Filter/FilterBuilder.h"
#include "../../Ecs/Systems/ISystem.h"
#include "../Components/PositionComponent.h"
#include "../Components/SpriteComponent.h"

class RenderSystem final : public ISystem
{
    sf::RenderWindow& _window;
    ComponentStorage<PositionComponent>& _positions;
    ComponentStorage<SpriteComponent>& _sprites;

    Filter _renderEntities;

public:
    RenderSystem(World& world, sf::RenderWindow& window)
        : ISystem(world),
          _window(window),
          _positions(world.GetStorage<PositionComponent>()),
          _sprites(world.GetStorage<SpriteComponent>()),
          _renderEntities(FilterBuilder(world)
              .With<PositionComponent>()
              .With<SpriteComponent>()
              .Build())
    {
    }

    void OnInit() override;
    void OnUpdate() override;
};

#endif //RENDERSYSTEM_H
