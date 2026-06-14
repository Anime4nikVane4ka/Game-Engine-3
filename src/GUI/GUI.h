#ifndef GUI_H
#define GUI_H

#include "../Ecs/World/World.h"
#include "../Sample/Systems/RenderSystem.h"

class GUI {
public:
    void Draw(World& world, RenderMode& renderMode, float levelTimeSeconds);

private:
    void DrawRenderModeControls(RenderMode& renderMode);
    void DrawEntities(World& world);
    void DrawHud(World& world, float levelTimeSeconds);

};

#endif // GUI_H
