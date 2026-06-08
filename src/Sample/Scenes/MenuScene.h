#ifndef MENUSCENE_H
#define MENUSCENE_H

#include "../../GameEngine/Scene.h"

class MenuScene final : public Scene
{
public:
    MenuScene(GameEngine& gameEngine);

    void Init() override;
    void Update(float delta) override;
};

#endif //MENUSCENE_H
