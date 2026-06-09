#ifndef ANIMATORCOMPONENT_H
#define ANIMATORCOMPONENT_H

#include <functional>
#include <map>
#include <string>

#include "../../GameEngine/Assets/Animation.h"

struct AnimatorComponent {
    std::map<std::string, std::reference_wrapper<const Animation>> Animations;
    int CurrentFrame = 0;
    int CurrentFrameDelay = 0;
    bool AnimationFinished = false;

    AnimatorComponent() = default;
};

#endif // ANIMATORCOMPONENT_H
