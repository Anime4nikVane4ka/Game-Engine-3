#include "InputSystem.h"
#include "iostream"

void InputSystem::OnInit()
{
}

bool IsAnyKeyPressed()
{
    const sf::Keyboard::Key keys[] = {
        sf::Keyboard::Key::A, sf::Keyboard::Key::B, sf::Keyboard::Key::C,
        sf::Keyboard::Key::D, sf::Keyboard::Key::E, sf::Keyboard::Key::F,
        sf::Keyboard::Key::H, sf::Keyboard::Key::I,
        sf::Keyboard::Key::J, sf::Keyboard::Key::K, sf::Keyboard::Key::L,
        sf::Keyboard::Key::M, sf::Keyboard::Key::N, sf::Keyboard::Key::O,
        sf::Keyboard::Key::P, sf::Keyboard::Key::Q, sf::Keyboard::Key::R,
        sf::Keyboard::Key::S, sf::Keyboard::Key::T, sf::Keyboard::Key::U,
        sf::Keyboard::Key::V, sf::Keyboard::Key::W, sf::Keyboard::Key::X,
        sf::Keyboard::Key::Y, sf::Keyboard::Key::Z, sf::Keyboard::Key::Num0,
        sf::Keyboard::Key::Num1, sf::Keyboard::Key::Num2, sf::Keyboard::Key::Num3,
        sf::Keyboard::Key::Num4, sf::Keyboard::Key::Num5, sf::Keyboard::Key::Num6,
        sf::Keyboard::Key::Num7, sf::Keyboard::Key::Num8, sf::Keyboard::Key::Num9,
        sf::Keyboard::Key::Space, sf::Keyboard::Key::Enter,
        sf::Keyboard::Key::Left, sf::Keyboard::Key::Right, sf::Keyboard::Key::Up,
        sf::Keyboard::Key::Down
    };

    for (const sf::Keyboard::Key key : keys)
    {
        if (sf::Keyboard::isKeyPressed(key))
            return true;
    }

    return false;
}

void InputSystem::OnUpdate()
{
    const bool guiToggleInputPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::G);
    if (guiToggleInputPressed && !_guiToggleInputWasPressed)
    {
        const int eventEntity = world.CreateEntity();
        _guiToggleEvents.Add(eventEntity, GuiToggleEvent());
    }
    _guiToggleInputWasPressed = guiToggleInputPressed;

    const bool pauseInputPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape);
    if (pauseInputPressed && !_pauseInputWasPressed)
    {
        const int eventEntity = world.CreateEntity();
        _pauseToggleEvents.Add(eventEntity, PauseToggleEvent());
    }
    _pauseInputWasPressed = pauseInputPressed;

    if (PauseState::IsPaused(_pauseStates))
        return;

    for (const auto gameStateEntity : _gameStateEntities)
    {
        if (_gameStates.Get(gameStateEntity).IsGameOver)
        {
            const bool restartInputPressed = IsAnyKeyPressed();
            if (restartInputPressed && !_restartInputWasPressed)
            {
                const int eventEntity = world.CreateEntity();
                _restartInputEvents.Add(eventEntity, RestartInputEvent());
            }

            _restartInputWasPressed = restartInputPressed;
            return;
        }
    }

    _restartInputWasPressed = false;

    const bool shoot = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space);

    sf::Vector2f direction(0.0f, 0.0f);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
    {
        direction.x -= 1.0f;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
    {
        direction.x += 1.0f;
    }

    for (const auto player : _players)
    {
        auto& movement = _movementComponents.Get(player);
        movement.Direction = direction;

        if (shoot)
        {
            const int eventEnt = world.CreateEntity();
            _shootInputEvents.Add(eventEnt, ShootInputEvent(player, true));
        }
    }
}
