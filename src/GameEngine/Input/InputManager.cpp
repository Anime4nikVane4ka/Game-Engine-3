#include "InputManager.h"

#include <imgui-SFML.h>

#include "../GameEngine.h"

InputManager::InputManager(sf::RenderWindow& window, GameEngine& gameEngine)
    : _window(window), _gameEngine(gameEngine)
{
}

void InputManager::ResetEndedActions()
{
    for (auto& [_, actionMap] : _actionKeyMaps)
    {
        for (auto& [_, action] : actionMap)
        {
            if (action != nullptr && action->Type() == End)
                action->Type() = None;
        }
    }

    for (auto& [_, actionMap] : _actionMouseBtnMaps)
    {
        for (auto& [_, action] : actionMap)
        {
            if (action != nullptr && action->Type() == End)
                action->Type() = None;
        }
    }
}

void InputManager::ResetTransientActions(const size_t scene)
{
    for (auto& [_, action] : _actionMouseWheelMaps[scene])
    {
        if (action != nullptr)
            action->Type() = None;
    }

    for (auto& [_, action] : _actionMouseMoveMaps[scene])
    {
        if (action != nullptr)
            action->Type() = None;
    }
}

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
    // ToDo: —брос end экшенов с прошлого кадра в none
    ResetEndedActions();
    ResetTransientActions(scene);

    // ToDo: цикл по эвентам окна и простановка их в соответсвующие экшены сцены
    while (const auto event = _window.pollEvent())
    {
        ImGui::SFML::ProcessEvent(_window, *event);

        if (event->is<sf::Event::Closed>())
        {
            _window.close();
            return false;
        }

        if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
        {
            const auto actionIterator = _actionKeyMaps[scene].find(keyPressed->code);
            if (actionIterator != _actionKeyMaps[scene].end() && actionIterator->second != nullptr)
                actionIterator->second->Type() = Start;
        }
        else if (const auto* keyReleased = event->getIf<sf::Event::KeyReleased>())
        {
            const auto actionIterator = _actionKeyMaps[scene].find(keyReleased->code);
            if (actionIterator != _actionKeyMaps[scene].end() && actionIterator->second != nullptr)
                actionIterator->second->Type() = End;
        }
        else if (const auto* mouseButtonPressed = event->getIf<sf::Event::MouseButtonPressed>())
        {
            const auto actionIterator = _actionMouseBtnMaps[scene].find(mouseButtonPressed->button);
            if (actionIterator != _actionMouseBtnMaps[scene].end() && actionIterator->second != nullptr)
            {
                actionIterator->second->Type() = Start;
                actionIterator->second->Value2() = mouseButtonPressed->position;
            }
        }
        else if (const auto* mouseButtonReleased = event->getIf<sf::Event::MouseButtonReleased>())
        {
            const auto actionIterator = _actionMouseBtnMaps[scene].find(mouseButtonReleased->button);
            if (actionIterator != _actionMouseBtnMaps[scene].end() && actionIterator->second != nullptr)
            {
                actionIterator->second->Type() = End;
                actionIterator->second->Value2() = mouseButtonReleased->position;
            }
        }
        else if (const auto* mouseWheelScrolled = event->getIf<sf::Event::MouseWheelScrolled>())
        {
            const auto actionIterator = _actionMouseWheelMaps[scene].find(mouseWheelScrolled->wheel);
            if (actionIterator != _actionMouseWheelMaps[scene].end() && actionIterator->second != nullptr)
            {
                actionIterator->second->Type() = Start;
                actionIterator->second->Value() = static_cast<short>(mouseWheelScrolled->delta);
                actionIterator->second->Value2() = mouseWheelScrolled->position;
            }
        }
        else if (const auto* mouseMoved = event->getIf<sf::Event::MouseMoved>())
        {
            const auto actionIterator = _actionMouseMoveMaps[scene].find(Move);
            if (actionIterator != _actionMouseMoveMaps[scene].end() && actionIterator->second != nullptr)
            {
                actionIterator->second->Type() = Start;
                actionIterator->second->Value2() = mouseMoved->position;
            }
        }
    }

    return _window.isOpen();
}
