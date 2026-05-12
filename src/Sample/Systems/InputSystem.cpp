#include "InputSystem.h"

void InputSystem::OnInit()
{
}

void InputSystem::OnUpdate()
{
    while (const std::optional event = _window.pollEvent())
    {
        if (event->is<sf::Event::Closed>())
        {
            _window.close();
        }
        else if (event->getIf<sf::Event::KeyPressed>())
        {
            const int eventEnt = world.CreateEntity();
            _eventComponents.Add(eventEnt, MoveInputEvent());
        }
    }
}
