#include "InputManager.h"

#include <imgui-SFML.h>
#include <iostream>

#include "../GameEngine.h"
#include "imgui.h"

void InputManager::RegisterInput(const size_t scene,
    const sf::Keyboard::Key key, std::shared_ptr<InputAction> action)
{
    _actionKeyMaps[scene][key] = action;
}

void InputManager::RegisterInput(const size_t scene,
    const sf::Mouse::Button btn, std::shared_ptr<InputAction> action)
{
    _actionMouseBtnMaps[scene][btn] = action;
}

void InputManager::RegisterInput(const size_t scene,
    const sf::Mouse::Wheel wheel, std::shared_ptr<InputAction> action)
{
    _actionMouseWheelMaps[scene][wheel] = action;
}

void InputManager::RegisterInput(const size_t scene, const MouseMove mv,
    std::shared_ptr<InputAction> action)
{
    _actionMouseMoveMaps[scene][mv] = action;
}

bool InputManager::ProcessInput(const size_t scene)
{
    // ToDo: Сброс end экшенов с прошлого кадра в none

    // ToDo: цикл по эвентам окна и простановка их в соответсвующие экшены сцены

    return res;
}
