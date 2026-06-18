#include "AnimationStateSystem.h"

#include <string>

bool IsShootState(const std::string& state, const PlayerAnimationStates& states) {
    return state == states.ShootIdle || state == states.ShootRun || state == states.ShootJump;
}

void AnimationStateSystem::OnInit() {}

void AnimationStateSystem::OnUpdate() {
    for (const int entity : _players) {
        auto& animationState = _animationStates.Get(entity);
        const auto& animator = _animators.Get(entity);
        const auto& movement = _movements.Get(entity);
        const bool shoot = _shooters.Has(entity) && _shooters.Get(entity).Shoot;

        if (IsShootState(animationState.CurrentState, _states) && !animator.AnimationFinished) {
            continue;
        }

        std::string newState = _states.Idle;
        if (shoot && movement.Direction.y != 0.0f) {
            newState = _states.ShootJump;
        } else if (shoot && movement.Direction.x != 0.0f) {
            newState = _states.ShootRun;
        } else if (shoot) {
            newState = _states.ShootIdle;
        } else if (movement.Direction.y != 0.0f) {
            newState = _states.Jump;
        } else if (movement.Direction.x != 0.0f) {
            newState = _states.Run;
        }

        if (animationState.CurrentState == newState) {
            continue;
        }

        animationState.NewState = newState;
        animationState.NeedChange = true;
    }
}
