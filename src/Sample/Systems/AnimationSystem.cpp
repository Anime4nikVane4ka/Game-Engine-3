#include "AnimationSystem.h"

void AnimationSystem::OnInit()
{
}

void AnimationSystem::OnUpdate()
{
    for (const int entity : _animatedEntities)
    {
        auto& state = _animationStates.Get(entity);
        auto& animator = _animators.Get(entity);
        auto& sprite = _sprites.Get(entity);

        if (state.NeedChange)
        {
            state.CurrentState = state.NewState;
            state.NeedChange = false;
            animator.CurrentFrame = 0;
            animator.CurrentFrameTime = 0;
        }

        const auto animationIterator = animator.Animations.find(state.CurrentState);
        if (animationIterator == animator.Animations.end())
            continue;

        const auto& animation = animationIterator->second.get();
        if (animation.FrameCount() == 0 || animation.FrameDuration() == 0)
            continue;

        sprite.Texture = &animation.GetTexture();
        sprite.TextureRect = sf::IntRect(
            {static_cast<int>(animator.CurrentFrame * animation.Size().x), 0},
            animation.Size());

        animator.CurrentFrameTime++;
        if (animator.CurrentFrameTime < animation.FrameDuration())
            continue;

        animator.CurrentFrameTime = 0;
        animator.CurrentFrame = (animator.CurrentFrame + 1) % animation.FrameCount();
    }
}
