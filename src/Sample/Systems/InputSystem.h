#ifndef INPUTSYSTEM_H
#define INPUTSYSTEM_H

#include <memory>
#include <utility>

#include "../../Ecs/Filter/Filter.h"
#include "../../Ecs/Filter/FilterBuilder.h"
#include "../../Ecs/Systems/ISystem.h"
#include "../../GameEngine/Input/InputAction.h"
#include "../Components/MovementComponent.h"
#include "../Components/PlayerComponent.h"
#include "../Components/ShooterComponent.h"

class InputSystem final : public ISystem
{
    ComponentStorage<MovementComponent>& _movementComponents;
    ComponentStorage<ShooterComponent>& _shooterComponents;

    Filter _players;

    std::shared_ptr<InputAction> _moveLeft;
    std::shared_ptr<InputAction> _moveRight;
    std::shared_ptr<InputAction> _jump;
    std::shared_ptr<InputAction> _shoot;
    bool _jumpWasActive = false;

    bool IsActive(const std::shared_ptr<InputAction>& action) const;

public:
    InputSystem(
        World& world,
        std::shared_ptr<InputAction> moveLeft,
        std::shared_ptr<InputAction> moveRight,
        std::shared_ptr<InputAction> jump,
        std::shared_ptr<InputAction> shoot)
        : ISystem(world),
          _movementComponents(world.GetStorage<MovementComponent>()),
          _shooterComponents(world.GetStorage<ShooterComponent>()),
          _players(FilterBuilder(world)
              .With<PlayerComponent>()
              .With<MovementComponent>()
              .Build()),
          _moveLeft(std::move(moveLeft)),
          _moveRight(std::move(moveRight)),
          _jump(std::move(jump)),
          _shoot(std::move(shoot))
    {
    }

    void OnInit() override;
    void OnUpdate() override;
};

#endif //INPUTSYSTEM_H
