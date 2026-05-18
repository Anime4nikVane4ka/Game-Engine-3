#ifndef ANIMATORCOMPONENT_H
#define ANIMATORCOMPONENT_H

#include <cstddef>
#include <functional>
#include <map>
#include <string>

#include "../../GameEngine/Assets/Animation.h"

struct AnimatorComponent
{
    std::map<std::string, std::reference_wrapper<const Animation>> Animations;
    std::size_t CurrentFrame = 0;
    std::size_t CurrentFrameTime = 0;

    AnimatorComponent() = default;
};

#endif //ANIMATORCOMPONENT_H
