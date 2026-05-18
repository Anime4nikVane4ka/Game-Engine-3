#include "FollowCameraSystem.h"

void FollowCameraSystem::OnInit()
{
    for (const int entity : _followCameras)
    {
        _cameras.Get(entity).View = _window.getDefaultView();
        return;
    }
}

void FollowCameraSystem::OnUpdate()
{
    for (const int cameraEntity : _followCameras)
    {
        auto& camera = _cameras.Get(cameraEntity);

        for (const int playerEntity : _players)
        {
            const auto& playerPosition = _positions.Get(playerEntity);
            const auto center = camera.View.getCenter();
            camera.View.setCenter({playerPosition.Position.x, center.y});
            _window.setView(camera.View);
            return;
        }
    }
}
