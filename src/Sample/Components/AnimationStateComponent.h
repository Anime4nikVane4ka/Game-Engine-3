#ifndef ANIMATIONSTATECOMPONENT_H
#define ANIMATIONSTATECOMPONENT_H

#include <string>

struct AnimationStateComponent {
    std::string CurrentState;
    std::string NewState;
    bool NeedChange = false;

    AnimationStateComponent() = default;
    AnimationStateComponent(const std::string& initialState)
        : CurrentState(initialState), NewState(initialState) {}
};

#endif // ANIMATIONSTATECOMPONENT_H
