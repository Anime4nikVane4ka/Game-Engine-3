#include "AnimationStateSystem.h"

namespace
{
    constexpr const char* IdleState = "Idle";
    constexpr const char* RunState = "Run";
    constexpr const char* JumpState = "Jump";
}

void AnimationStateSystem::OnInit()
{
}

void AnimationStateSystem::OnUpdate()
{
    for (const int entity : _players)
    {
        auto& animationState = _animationStates.Get(entity);
        const auto& movement = _movements.Get(entity);

        const char* newState = IdleState;
        if (movement.Direction.y != 0.0f)
            newState = JumpState;
        else if (movement.Direction.x != 0.0f)
            newState = RunState;

        if (animationState.CurrentState == newState)
            continue;

        animationState.NewState = newState;
        animationState.NeedChange = true;
    }
}
