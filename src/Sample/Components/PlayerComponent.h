#ifndef PLAYERCOMPONENT_H
#define PLAYERCOMPONENT_H

struct PlayerComponent
{
    int Score = 0;

    PlayerComponent() = default;
    void AddScore(const int value) { Score += value; }
    void ResetScore() { Score = 0; }
};

#endif //PLAYERCOMPONENT_H
