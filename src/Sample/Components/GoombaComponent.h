#ifndef GOOMBACOMPONENT_H
#define GOOMBACOMPONENT_H

struct GoombaComponent {
    bool IsChasing = false;
    float ViewDistance = 512.0f;

    GoombaComponent() = default;
    GoombaComponent(const bool isChasing, const float viewDistance)
        : IsChasing(isChasing), ViewDistance(viewDistance) {}
};

#endif // GOOMBACOMPONENT_H
