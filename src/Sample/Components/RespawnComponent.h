
#ifndef ECS_DEMO_RESPAWNCOMPONENT_H
#define ECS_DEMO_RESPAWNCOMPONENT_H
#include <SFML/System/Vector2.hpp>

struct RespawnComponent {
    sf::Vector2f Position;
    bool NeedRespawn = false;

    RespawnComponent() = default;
    explicit RespawnComponent(const sf::Vector2f& position) : Position(position) {}
};

#endif // ECS_DEMO_RESPAWNCOMPONENT_H
