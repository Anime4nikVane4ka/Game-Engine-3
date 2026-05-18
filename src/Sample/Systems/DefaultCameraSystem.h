#ifndef DEFAULTCAMERASYSTEM_H
#define DEFAULTCAMERASYSTEM_H

#include <SFML/Graphics/RenderWindow.hpp>

#include "../../Ecs/Filter/Filter.h"
#include "../../Ecs/Filter/FilterBuilder.h"
#include "../../Ecs/Systems/ISystem.h"
#include "../Components/CameraComponent.h"
#include "../Components/DefaultCameraComponent.h"

class DefaultCameraSystem final : public ISystem
{
    sf::RenderWindow& _window;
    ComponentStorage<CameraComponent>& _cameras;

    Filter _defaultCameras;

public:
    DefaultCameraSystem(World& world, sf::RenderWindow& window)
        : ISystem(world),
          _window(window),
          _cameras(world.GetStorage<CameraComponent>()),
          _defaultCameras(FilterBuilder(world)
              .With<CameraComponent>()
              .With<DefaultCameraComponent>()
              .Build())
    {
    }

    void OnInit() override;
    void OnUpdate() override;
};

#endif //DEFAULTCAMERASYSTEM_H
