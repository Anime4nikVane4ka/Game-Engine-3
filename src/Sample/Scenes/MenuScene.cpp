#include "MenuScene.h"
#include <imgui.h>
#include <SFML/Graphics/Color.hpp>

#include "../../GameEngine/GameEngine.h"
#include "GameScene.h"

MenuScene::MenuScene(GameEngine& gameEngine) : Scene(gameEngine) {}

void MenuScene::Init() {}

void MenuScene::Update(float delta) {
    (void)delta;
    gameEngine.Window().clear(sf::Color::Black);
    ImGui::SetNextWindowPos(ImVec2(0, 0));

    ImGui::SetNextWindowSize(ImVec2(
       static_cast<float>(GameEngineConfiguration::Width),
       static_cast<float>(GameEngineConfiguration::Height)
   ));

    ImGui::Begin("Menu", nullptr,
       ImGuiWindowFlags_NoTitleBar |
       ImGuiWindowFlags_NoResize |
       ImGuiWindowFlags_NoMove |
       ImGuiWindowFlags_NoCollapse
   );
    ImGui::SetCursorPos(ImVec2(540, 280));

    // Рисуем кнопку Play размером 200x80.
    // Если пользователь нажал кнопку, загружаем игровую сцену.
    if (ImGui::Button("Play", ImVec2(200, 80))) {
        gameEngine.LoadScene<GameScene>(gameEngine);
    }

    // Ставим курсор ниже, где будет кнопка Exit.
    ImGui::SetCursorPos(ImVec2(540, 380));

    // Рисуем кнопку Exit.
    // Если пользователь нажал кнопку, закрываем игру.
    if (ImGui::Button("Exit", ImVec2(200, 80))) {
        gameEngine.Quit();
    }
    ImGui::End();
}
