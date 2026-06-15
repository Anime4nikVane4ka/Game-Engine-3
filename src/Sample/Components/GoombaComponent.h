#ifndef GOOMBACOMPONENT_H
#define GOOMBACOMPONENT_H

struct GoombaComponent {
    bool IsChasing = false;
    bool IsReturningToPatrol = false;
    float WaitBeforePatrolMs = 0.0f;
    float MaxWaitBeforePatrolMs = 5000.0f;
    float ViewDistance = 512.0f;

    GoombaComponent() = default;
    GoombaComponent(const bool isChasing, const float viewDistance) : IsChasing(isChasing), ViewDistance(viewDistance) {}
};

#endif // GOOMBACOMPONENT_H
