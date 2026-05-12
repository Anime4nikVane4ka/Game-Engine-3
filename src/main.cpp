#include <iostream>
#include <SFML/Graphics.hpp>

#include "GameEngine/GameEngine.h"
#include "Ecs/Systems/SystemsManager.h"
#include "Ecs/World/World.h"
#include "GameEngine/Input/InputManager.h"

#include "Sample/Systems/InitSystem.h"
#include "Sample/Systems/InputSystem.h"
#include "Sample/Systems/MovementSystem.h"
#include "Sample/Systems/RenderSystem.h"

int main() {
    setlocale(LC_ALL, "");

    GameEngine engine(GameEngineConfiguration {});
    engine.Run();

    /* Old way of doing
    InputManager inputManager(window);
    inputManager._currentActionMap = "ao";
    std::map<std::string, std::shared_ptr<InputAction>> actionMap;
    auto ia = std::make_shared<InputAction>("WWWW");
    actionMap.insert(std::pair(ia->Name(), ia));
    inputManager.RegisterInput(sf::Keyboard::Key::W, ia);
    World world;
    SystemsManager systems(world);
    systems.AddInitializer(std::make_shared<InitSystem>(world));
    //systems.AddSystem(std::make_shared<InputSystem>(world, window));
    systems.AddSystem(std::make_shared<MovementSystem>(world));
    systems.AddSystem(std::make_shared<RenderSystem>(world, window));

    while (window.isOpen())
    {
        //bool a = inputManager.ProcessInput();
        //if (a)
        //    std::cout << "frame" << std::endl;
        systems.Update();
    }
    */
}