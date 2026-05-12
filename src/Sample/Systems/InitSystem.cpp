#include "InitSystem.h"

#include "../Components/PositionComponent.h"
#include "../Components/MovementComponent.h"

void InitSystem::OnInit()
{
    const int player = world.CreateEntity();
    auto& positionsStorage = world.GetStorage<PositionComponent>();
    auto& movementsStorage = world.GetStorage<MovementComponent>();

    positionsStorage.Add(player, PositionComponent(0, 0));
    movementsStorage.Add(player, MovementComponent(5, sf::Vector2f(0, 1)));
}
