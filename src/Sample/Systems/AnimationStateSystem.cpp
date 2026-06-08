#include "AnimationStateSystem.h"

#include <string>

constexpr const char* IdleState = "Idle";
constexpr const char* RunState = "Run";
constexpr const char* JumpState = "Jump";
constexpr const char* ShootIdleState = "ShootIdle";
constexpr const char* ShootRunState = "ShootRun";
constexpr const char* ShootJumpState = "ShootJump";

bool IsShootState(const std::string& state)
{
    return state == ShootIdleState || state == ShootRunState || state == ShootJumpState;
}

void AnimationStateSystem::OnInit()
{
}

void AnimationStateSystem::OnUpdate()
{
    for (const int entity : _players)
    {
        auto& animationState = _animationStates.Get(entity);
        const auto& animator = _animators.Get(entity);
        const auto& movement = _movements.Get(entity);
        const bool shoot = _shooters.Has(entity) && _shooters.Get(entity).Shoot;

        if (IsShootState(animationState.CurrentState) && !animator.AnimationFinished)
            continue;

        const char* newState = IdleState;
        if (shoot && movement.Direction.y != 0.0f)
            newState = ShootJumpState;
        else if (shoot && movement.Direction.x != 0.0f)
            newState = ShootRunState;
        else if (shoot)
            newState = ShootIdleState;
        else if (movement.Direction.y != 0.0f)
            newState = JumpState;
        else if (movement.Direction.x != 0.0f)
            newState = RunState;

        if (animationState.CurrentState == newState)
            continue;

        animationState.NewState = newState;
        animationState.NeedChange = true;
    }
}
