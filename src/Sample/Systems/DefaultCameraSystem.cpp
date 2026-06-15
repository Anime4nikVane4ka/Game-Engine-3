#include "DefaultCameraSystem.h"

void DefaultCameraSystem::OnInit() {
    for (const int entity : _defaultCameras) {
        _cameras.Get(entity).View = _window.getDefaultView();
        return;
    }
}

void DefaultCameraSystem::OnUpdate() {
    for (const int entity : _defaultCameras) {
        _window.setView(_cameras.Get(entity).View);
        return;
    }
}
