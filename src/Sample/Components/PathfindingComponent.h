#ifndef PATHFINDINGCOMPONENT_H
#define PATHFINDINGCOMPONENT_H

#include <vector>

#include <SFML/System/Vector2.hpp>

struct PathfindingComponent {
    std::vector<sf::Vector2i> Path;
    int CurrentPathIndex = 0;
    bool NeedUpdate = true;

    PathfindingComponent() = default;
};

#endif // PATHFINDINGCOMPONENT_H
