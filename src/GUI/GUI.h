#ifndef GUI_H
#define GUI_H

#include "../Ecs/World/World.h"
#include "../Sample/Systems/RenderSystem.h"
#include "../GameEngine/Assets/AssetManager.h"

class GUI {
public:
    void Draw(World& world, RenderMode& renderMode, float levelTimeSeconds, const AssetManager& assetManager);

private:
    void DrawRenderModeControls(RenderMode& renderMode);
    void DrawEntities(World& world);
    void DrawHud(World& world, float levelTimeSeconds);
    void DrawAssets(const AssetManager& assetManager);

};

#endif // GUI_H
