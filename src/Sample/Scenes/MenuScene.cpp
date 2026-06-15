#include "MenuScene.h"
#include "../BestTimeStorage.h"
#include <SFML/Graphics/Color.hpp>
#include <imgui.h>

#include "../../GameEngine/GameEngine.h"
#include "../Components/CameraComponent.h"
#include "../Components/DefaultCameraComponent.h"
#include "../Systems/DefaultCameraSystem.h"
#include "GameScene.h"
#ifdef ENABLE_LEVEL_EDITOR
#include "EditorScene.h"
#endif

MenuScene::MenuScene(GameEngine& gameEngine) : Scene(gameEngine) {}

void MenuScene::Init() {
    systemsManager.AddSystem(std::make_shared<DefaultCameraSystem>(world, gameEngine.Window()));

    auto& cameras = world.GetStorage<CameraComponent>();
    auto& defaultCameras = world.GetStorage<DefaultCameraComponent>();

    const int camera = world.CreateEntity();
    cameras.Add(camera, CameraComponent());
    defaultCameras.Add(camera, DefaultCameraComponent());
}

void MenuScene::Update(float delta) {
    (void)delta;
    gameEngine.Window().clear(sf::Color::Black);
    systemsManager.Update();
    ImGui::SetNextWindowPos(ImVec2(0, 0));

    ImGui::SetNextWindowSize(ImVec2(static_cast<float>(GameEngineConfiguration::Width), static_cast<float>(GameEngineConfiguration::Height)));

    ImGui::Begin("Menu", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);
    ImGui::SetCursorPos(ImVec2(540, 280));

    const float bestTime = BestTimeStorage::Load();

    ImGui::SetCursorPos(ImVec2(540, 220));

    if (bestTime >= 0.0f) {
        ImGui::Text("Best time: %.2f", bestTime);
    } else {
        ImGui::Text("Best time: --");
    }

    ImGui::SetCursorPos(ImVec2(540, 280));

    if (ImGui::Button("Play", ImVec2(200, 80))) {
        gameEngine.LoadScene<GameScene>(gameEngine);
    }

#ifdef ENABLE_LEVEL_EDITOR
    ImGui::SetCursorPos(ImVec2(540, 370));

    if (ImGui::Button("Editor", ImVec2(200, 80))) {
        gameEngine.LoadScene<EditorScene>(gameEngine);
    }
#endif

    // Ставим курсор ниже, где будет кнопка Exit.
    ImGui::SetCursorPos(ImVec2(540, 460));

    // Рисуем кнопку Exit.
    // Если пользователь нажал кнопку, закрываем игру.
    if (ImGui::Button("Exit", ImVec2(200, 80))) {
        gameEngine.Quit();
    }
    ImGui::End();
}
