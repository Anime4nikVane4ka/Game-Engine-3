#include "MenuScene.h"
#include "../BestTimeStorage.h"
#include <SFML/Graphics/Color.hpp>
#include <imgui.h>

#include "../../GameEngine/GameEngine.h"
#include "../Components/CameraComponent.h"
#include "../Components/DefaultCameraComponent.h"
#include "../Systems/DefaultCameraSystem.h"
#include "GameScene.h"
#include "../../GameEngine/GameEngineConfiguration.h"
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

    gameEngine.Window().clear(sf::Color(30, 40, 90));
    systemsManager.Update();

    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImVec2(
        static_cast<float>(GameEngineConfiguration::Width),
        static_cast<float>(GameEngineConfiguration::Height)
    ));

    ImGui::Begin("Menu", nullptr,
        ImGuiWindowFlags_NoTitleBar |
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoScrollbar
    );

    ImDrawList* drawList = ImGui::GetWindowDrawList();

    const ImVec2 windowPos = ImGui::GetWindowPos();
    const ImVec2 windowSize = ImGui::GetWindowSize();

    drawList->AddRectFilled(
        windowPos,
        ImVec2(windowPos.x + windowSize.x, windowPos.y + windowSize.y),
        IM_COL32(40, 60, 150, 255)
    );

    drawList->AddCircleFilled(ImVec2(180, 140), 70.0f, IM_COL32(255, 255, 255, 60));
    drawList->AddCircleFilled(ImVec2(1020, 180), 90.0f, IM_COL32(255, 255, 255, 45));
    drawList->AddCircleFilled(ImVec2(620, 560), 160.0f, IM_COL32(20, 160, 80, 160));

    ImGui::SetWindowFontScale(3.0f);

    const char* title = "MEGAMAN LEVELS";
    const ImVec2 titleSize = ImGui::CalcTextSize(title);

    ImGui::SetCursorPos(ImVec2(
        (GameEngineConfiguration::Width - titleSize.x) * 0.5f,
        120.0f
    ));

    ImGui::Text("%s", title);

    ImGui::SetWindowFontScale(1.0f);

    const float buttonWidth = 240.0f;
    const float buttonHeight = 70.0f;
    const float buttonX = (GameEngineConfiguration::Width - buttonWidth) * 0.5f;

    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 10.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 2.0f);

    ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(70, 130, 220, 255));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, IM_COL32(100, 170, 255, 255));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, IM_COL32(40, 100, 190, 255));
    ImGui::PushStyleColor(ImGuiCol_Border, IM_COL32(255, 255, 255, 180));

    ImGui::SetCursorPos(ImVec2(buttonX, 280.0f));

    if (ImGui::Button("Play", ImVec2(buttonWidth, buttonHeight))) {
        gameEngine.LoadScene<GameScene>(gameEngine);
    }

#ifdef ENABLE_LEVEL_EDITOR
    ImGui::SetCursorPos(ImVec2(buttonX, 370.0f));

    if (ImGui::Button("Editor", ImVec2(buttonWidth, buttonHeight))) {
        gameEngine.LoadScene<EditorScene>(gameEngine);
    }
#endif

    ImGui::SetCursorPos(ImVec2(buttonX, 460.0f));

    if (ImGui::Button("Exit", ImVec2(buttonWidth, buttonHeight))) {
        gameEngine.Quit();
    }

    ImGui::PopStyleColor(4);
    ImGui::PopStyleVar(2);

    const float bestTime = BestTimeStorage::Load();

    ImGui::SetCursorPos(ImVec2(buttonX, 560.0f));

    if (bestTime >= 0.0f) {
        ImGui::Text("Best time: %.2f", bestTime);
    } else {
        ImGui::Text("Best time: --");
    }

    ImGui::End();
}

