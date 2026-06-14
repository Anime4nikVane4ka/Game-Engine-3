#include "GUI.h"

#include <string>
#include <algorithm>
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
#include <imgui-SFML.h>
#include <SFML/System/Vector2.hpp>

namespace {
sf::Vector2f FitImageSize(const sf::Vector2u& textureSize, const float maxSize) {
    if (textureSize.x == 0 || textureSize.y == 0) {
        return {maxSize, maxSize};
    }

    const float width = static_cast<float>(textureSize.x);
    const float height = static_cast<float>(textureSize.y);

    const float scale = maxSize / std::max(width, height);

    return {width * scale, height * scale};
}

sf::Vector2f FitImageSize(const sf::Vector2i& textureSize, const float maxSize) {
    return FitImageSize(sf::Vector2u(
        static_cast<unsigned int>(textureSize.x),
        static_cast<unsigned int>(textureSize.y)
    ), maxSize);
}
}


void GUI::Draw(World& world,
    RenderMode& renderMode,
    float levelTimeSeconds,
    const AssetManager& assetManager) {
    DrawHud(world, levelTimeSeconds);

    ImGui::Begin("Game UI");

    if (ImGui::BeginTabBar("GameUiTabs")) {
        if (ImGui::BeginTabItem("Render")) {
            DrawRenderModeControls(renderMode);
            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("Entities")) {
            DrawEntities(world);
            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("Assets")) {
            DrawAssets(assetManager);
            ImGui::EndTabItem();
        }

        ImGui::EndTabBar();
    }

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
void GUI::DrawAssets(const AssetManager& assetManager) {
    if (ImGui::BeginTabBar("AssetsTabs")) {
        if (ImGui::BeginTabItem("Textures")) {
            for (const auto& [name, texture] : assetManager.GetTextures()) {
                ImGui::PushID(name.c_str());

                if (ImGui::ImageButton(name.c_str(), texture, FitImageSize(texture.getSize(), 64.0f))) {
                }

                ImGui::SameLine();
                ImGui::Text("%s", name.c_str());

                ImGui::PopID();
            }

            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("Animations")) {
            for (const auto& [name, animation] : assetManager.GetAnimations()) {
                ImGui::PushID(name.c_str());

                if (ImGui::ImageButton(name.c_str(),
                    animation.GetTexture(),
                    FitImageSize(animation.Size(), 64.0f))) {
                    }

                ImGui::SameLine();
                ImGui::Text("%s", name.c_str());

                ImGui::PopID();
            }

            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("Fonts")) {
            for (const auto& [name, font] : assetManager.GetFonts()) {
                (void)font;

                if (ImGui::Button(name.c_str(), ImVec2(160.0f, 40.0f))) {
                }
            }

            ImGui::EndTabItem();
        }
        ImGui::EndTabBar();
    }

}