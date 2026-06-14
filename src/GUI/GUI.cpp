#include "GUI.h"

#include <string>

#include <imgui.h>

#include "../Sample/Components/BrickTileComponent.h"
#include "../Sample/Components/BulletComponent.h"
#include "../Sample/Components/CameraComponent.h"
#include "../Sample/Components/DecorComponent.h"
#include "../Sample/Components/DefaultCameraComponent.h"
#include "../Sample/Components/FinishComponent.h"
#include "../Sample/Components/FollowXCameraComponent.h"
#include "../Sample/Components/PlayerComponent.h"
#include "../Sample/Components/PositionComponent.h"
#include "../Sample/Components/QuestionTileComponent.h"
#include "../Sample/Components/TileComponent.h"

void GUI::Draw(World& world, RenderMode& renderMode, float levelTimeSeconds) {
    DrawHud(world, levelTimeSeconds);

    ImGui::Begin("Game UI");

    DrawRenderModeControls(renderMode);

    ImGui::Separator();

    DrawEntities(world);

    ImGui::End();
}

void GUI::DrawRenderModeControls(RenderMode& renderMode) {
    ImGui::Text("Render mode");

    if (ImGui::Button("Textures")) {
        renderMode = RenderMode::Textures;
    }

    ImGui::SameLine();

    if (ImGui::Button("Colliders")) {
        renderMode = RenderMode::Colliders;
    }

    ImGui::SameLine();

    if (ImGui::Button("Grid")) {
        renderMode = RenderMode::Grid;
    }
}

void GUI::DrawHud(World& world, float levelTimeSeconds) {
    ImGui::SetNextWindowPos(ImVec2(10, 10));
    ImGui::SetNextWindowBgAlpha(0.35f);

    ImGui::Begin("HUD", nullptr,
        ImGuiWindowFlags_NoTitleBar |
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_AlwaysAutoResize |
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoCollapse
    );

    ImGui::Text("Time: %.2f", levelTimeSeconds);

    auto& players = world.GetStorage<PlayerComponent>();
    for (const int player : players.Entities()) {
        ImGui::Text("Score: %d", players.Get(player).Score);
        break;
    }

    ImGui::End();
}

void GUI::DrawEntities(World& world) {
    ImGui::Text("Entities");

    auto& positions = world.GetStorage<PositionComponent>();

    auto& players = world.GetStorage<PlayerComponent>();
    auto& tiles = world.GetStorage<TileComponent>();
    auto& bricks = world.GetStorage<BrickTileComponent>();
    auto& finishes = world.GetStorage<FinishComponent>();
    auto& decorations = world.GetStorage<DecorComponent>();
    auto& bullets = world.GetStorage<BulletComponent>();
    auto& cameras = world.GetStorage<CameraComponent>();
    auto& followCameras = world.GetStorage<FollowXCameraComponent>();
    auto& defaultCameras = world.GetStorage<DefaultCameraComponent>();
    auto& questionTiles = world.GetStorage<QuestionTileComponent>();

    const auto& componentStorages = world.ComponentStorages();

    for (const auto& entity : world.Entities()) {
        if (entity.IsRemoved()) {
            continue;
        }

        const int entityId = entity.Id;

        std::string flags;

        auto addFlag = [&flags](const char* name) {
            if (!flags.empty()) {
                flags += ", ";
            }

            flags += name;
        };

        if (players.Has(entityId)) addFlag("Player");
        if (tiles.Has(entityId)) addFlag("Tile");
        if (bricks.Has(entityId)) addFlag("Brick");
        if (finishes.Has(entityId)) addFlag("Finish");
        if (decorations.Has(entityId)) addFlag("Decor");
        if (bullets.Has(entityId)) addFlag("Bullet");
        if (cameras.Has(entityId)) addFlag("Camera");
        if (followCameras.Has(entityId)) addFlag("FollowXCamera");
        if (defaultCameras.Has(entityId)) addFlag("DefaultCamera");
        if (questionTiles.Has(entityId)) addFlag("QuestionTile");

        std::string title = "Entity " + std::to_string(entityId);

        if (!flags.empty()) {
            title += " (" + flags + ")";
        }

        ImGui::PushID(entityId);

        if (ImGui::TreeNode(title.c_str())) {
            if (positions.Has(entityId)) {
                const auto& position = positions.Get(entityId).Position;
                ImGui::Text("Position: %.1f, %.1f", position.x, position.y);
            } else {
                ImGui::Text("Position: none");
            }

            if (ImGui::TreeNode("Components")) {
                for (const auto& storage : componentStorages) {
                    if (storage->Has(entityId)) {
                        ImGui::BulletText("%s", storage->TypeName());
                    }
                }

                ImGui::TreePop();
            }

            ImGui::TreePop();
        }

        ImGui::PopID();
    }
}