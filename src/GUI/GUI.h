#ifndef GUI_H
#define GUI_H

#include "../Ecs/World/World.h"

class GUI
{
public:
    GUI() = default;

    void ToggleCollapsed();
    void Draw(World& world);

private:
    bool _collapsed = false;

    void DrawAsteroidSpawnControls(World& world);
    void DrawUfoSpawnControls(World& world);
    void DrawEntities(World& world);
};

#endif //GUI_H
