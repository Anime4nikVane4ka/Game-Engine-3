#ifndef GAMESCENE_H
#define GAMESCENE_H

#include "../../GameEngine/Scene.h"

class GameScene final : public Scene
{
public:
    GameScene(GameEngine& gameEngine);

    void Init() override;
    void Update(float delta) override;
};

#endif //GAMESCENE_H
