#ifndef FOLLOWCAMERASYSTEM_H
#define FOLLOWCAMERASYSTEM_H

#include <SFML/Graphics/RenderWindow.hpp>

#include "../../Ecs/Filter/Filter.h"
#include "../../Ecs/Filter/FilterBuilder.h"
#include "../../Ecs/Systems/ISystem.h"
#include "../Components/CameraComponent.h"
#include "../Components/FollowXCameraComponent.h"
#include "../Components/PlayerComponent.h"
#include "../Components/PositionComponent.h"

class FollowCameraSystem final : public ISystem {
    sf::RenderWindow& _window;
    ComponentStorage<CameraComponent>& _cameras;
    ComponentStorage<PositionComponent>& _positions;

    Filter _followCameras;
    Filter _players;

  public:
    FollowCameraSystem(World& world, sf::RenderWindow& window)
        : ISystem(world), _window(window), _cameras(world.GetStorage<CameraComponent>()),
          _positions(world.GetStorage<PositionComponent>()),
          _followCameras(
              FilterBuilder(world).With<CameraComponent>().With<FollowXCameraComponent>().Build()),
          _players(FilterBuilder(world).With<PlayerComponent>().With<PositionComponent>().Build()) {
    }

    void OnInit() override;
    void OnUpdate() override;
};

#endif // FOLLOWCAMERASYSTEM_H
