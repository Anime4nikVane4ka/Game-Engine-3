#ifndef GAMESCENE_H
#define GAMESCENE_H

#include "../../GUI/GUI.h"
#include "../../GameEngine/Scene.h"
#include "../Systems/RenderSystem.h"

class GameScene final : public Scene {
    bool _paused = false;
    bool _pauseWasActive = false;
    RenderMode _renderMode = RenderMode::Textures;

    GUI _gui;

    float _levelTimeSeconds = 0.0f;

  public:
    GameScene(GameEngine& gameEngine);

    void Init() override;
    void Update(float delta) override;
};

#endif // GAMESCENE_H
