#ifndef DESTROYONANIMATIONENDCOMPONENT_H
#define DESTROYONANIMATIONENDCOMPONENT_H

struct DestroyOnAnimationEndComponent {
    int FramesRemaining = -1;

    DestroyOnAnimationEndComponent() = default;
    explicit DestroyOnAnimationEndComponent(const int framesRemaining) : FramesRemaining(framesRemaining) {}
};

#endif // DESTROYONANIMATIONENDCOMPONENT_H
