#include "AnimationSystem.h"

void AnimationSystem::OnInit() {}

void AnimationSystem::OnUpdate() {
    for (const int entity : _animatedEntities) {
        auto& state = _animationStates.Get(entity);
        auto& animator = _animators.Get(entity);
        auto& sprite = _sprites.Get(entity);

        if (state.NeedChange) {
            state.CurrentState = state.NewState;
            state.NeedChange = false;
            animator.CurrentFrame = 0;
            animator.CurrentFrameDelay = 0;
            animator.AnimationFinished = false;
        }

        const auto animationIterator = animator.Animations.find(state.CurrentState);
        if (animationIterator == animator.Animations.end()) {
            continue;
        }

        const auto& animation = animationIterator->second.get();
        if (animation.FrameCount() <= 0 || animation.FrameDuration() <= 0) {
            continue;
        }

        sprite.Texture = &animation.GetTexture();
        sprite.TextureRect = sf::IntRect({animator.CurrentFrame * animation.Size().x, 0}, animation.Size());

        ++animator.CurrentFrameDelay;
        if (animator.CurrentFrameDelay >= animation.FrameDuration()) {
            animator.CurrentFrameDelay = 0;
            animator.CurrentFrame++;

            if (animator.CurrentFrame >= animation.FrameCount()) {
                animator.CurrentFrame = 0;
                animator.AnimationFinished = true;
            }
        }
    }
}
