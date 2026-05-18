#ifndef INPUTSYSTEM_H
#define INPUTSYSTEM_H

#include <SFML/Graphics.hpp>

#include "../../Ecs/Filter/Filter.h"
#include "../../Ecs/Filter/FilterBuilder.h"
#include "../../Ecs/Systems/ISystem.h"

#include "../Components/MovementComponent.h"
#include "../Components/PauseStateComponent.h"
#include "../Components/PauseToggleEvent.h"
#include "../Components/PlayerComponent.h"
#include "../Components/GameStateComponent.h"
#include "../Components/GuiToggleEvent.h"
#include "../Components/RestartInputEvent.h"
#include "../Components/ShootInputEvent.h"

class InputSystem : public ISystem {

    ComponentStorage<ShootInputEvent>& _shootInputEvents;
    ComponentStorage<RestartInputEvent>& _restartInputEvents;
    ComponentStorage<GuiToggleEvent>& _guiToggleEvents;
    ComponentStorage<PauseToggleEvent>& _pauseToggleEvents;
    ComponentStorage<PauseStateComponent>& _pauseStates;
    ComponentStorage<MovementComponent>& _movementComponents;
    ComponentStorage<GameStateComponent>& _gameStates;
    Filter _players;
    Filter _gameStateEntities;
    bool _restartInputWasPressed = false;
    bool _guiToggleInputWasPressed = false;
    bool _pauseInputWasPressed = false;


public:
    InputSystem(World &world)
        : ISystem(world),
        _shootInputEvents(world.GetStorage<ShootInputEvent>()),
        _restartInputEvents(world.GetStorage<RestartInputEvent>()),
        _guiToggleEvents(world.GetStorage<GuiToggleEvent>()),
        _pauseToggleEvents(world.GetStorage<PauseToggleEvent>()),
        _pauseStates(world.GetStorage<PauseStateComponent>()),
        _movementComponents(world.GetStorage<MovementComponent>()),
        _gameStates(world.GetStorage<GameStateComponent>()),
        _players(FilterBuilder(world)
            .With<PlayerComponent>()
            .With<MovementComponent>()
            .Build()),
        _gameStateEntities(FilterBuilder(world)
            .With<GameStateComponent>()
            .Build())
    {

    }

    void OnInit() override;

    void OnUpdate() override;
};



#endif //INPUTSYSTEM_H
