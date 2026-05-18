#include "GUI.h"

#include <string>

#include "imgui.h"

#include "../Sample/Components/AsteroidSpawnRequestEvent.h"
#include "../Sample/Components/AsteroidSpawnSettingsComponent.h"
#include "../Sample/Components/PositionComponent.h"
#include "../Sample/Components/UfoSpawnRequestEvent.h"
#include "../Sample/Components/UfoSpawnSettingsComponent.h"

void GUI::ToggleCollapsed()
{
    _collapsed = !_collapsed;
}

void GUI::Draw(World& world)
{
    ImGui::SetNextWindowCollapsed(_collapsed, ImGuiCond_Always);
    ImGui::Begin("Game UI");

    DrawAsteroidSpawnControls(world);
    ImGui::Separator();
    DrawUfoSpawnControls(world);
    ImGui::Separator();
    DrawEntities(world);

    _collapsed = ImGui::IsWindowCollapsed();
    ImGui::End();
}

void GUI::DrawAsteroidSpawnControls(World& world)
{
    auto& spawnSettings = world.GetStorage<AsteroidSpawnSettingsComponent>();
    int settingsEntity = -1;
    for (const int entity : spawnSettings.Entities())
    {
        settingsEntity = entity;
        break;
    }

    if (settingsEntity >= 0)
    {
        auto& settings = spawnSettings.Get(settingsEntity);

        ImGui::Text("Asteroid spawn");
        ImGui::SliderFloat("Min period, ms", &settings.MinSpawnPeriodMs, 1.0f, 5000.0f, "%.0f");
        ImGui::SliderFloat("Max period, ms", &settings.MaxSpawnPeriodMs, 1.0f, 5000.0f, "%.0f");

        if (settings.MinSpawnPeriodMs < 1.0f)
            settings.MinSpawnPeriodMs = 1.0f;
        if (settings.MaxSpawnPeriodMs < settings.MinSpawnPeriodMs)
            settings.MaxSpawnPeriodMs = settings.MinSpawnPeriodMs;
    }
    else
    {
        ImGui::Text("Asteroid spawn settings are not initialized");
    }

    if (ImGui::Button("Spawn asteroid"))
    {
        const int eventEntity = world.CreateEntity();
        world.GetStorage<AsteroidSpawnRequestEvent>().Add(eventEntity, AsteroidSpawnRequestEvent());
    }
}

void GUI::DrawUfoSpawnControls(World& world)
{
    auto& spawnSettings = world.GetStorage<UfoSpawnSettingsComponent>();
    int settingsEntity = -1;
    for (const int entity : spawnSettings.Entities())
    {
        settingsEntity = entity;
        break;
    }

    if (settingsEntity >= 0)
    {
        auto& settings = spawnSettings.Get(settingsEntity);

        ImGui::Text("UFO spawn");
        ImGui::SliderFloat("UFO min period, ms", &settings.MinSpawnPeriodMs, 1.0f, 15000.0f, "%.0f");
        ImGui::SliderFloat("UFO max period, ms", &settings.MaxSpawnPeriodMs, 1.0f, 15000.0f, "%.0f");

        if (settings.MinSpawnPeriodMs < 1.0f)
            settings.MinSpawnPeriodMs = 1.0f;
        if (settings.MaxSpawnPeriodMs < settings.MinSpawnPeriodMs)
            settings.MaxSpawnPeriodMs = settings.MinSpawnPeriodMs;
    }
    else
    {
        ImGui::Text("UFO spawn settings are not initialized");
    }

    if (ImGui::Button("Spawn UFO"))
    {
        const int eventEntity = world.CreateEntity();
        world.GetStorage<UfoSpawnRequestEvent>().Add(eventEntity, UfoSpawnRequestEvent());
    }
}

void GUI::DrawEntities(World& world)
{
    auto& positions = world.GetStorage<PositionComponent>();
    const auto& componentStorages = world.ComponentStorages();

    ImGui::Text("Entities");
    for (const auto& entity : world.Entities())
    {
        if (entity.IsRemoved())
            continue;

        const int entityId = entity.Id;
        const std::string title = "Entity " + std::to_string(entityId);

        ImGui::PushID(entityId);
        if (ImGui::TreeNode(title.c_str()))
        {
            if (positions.Has(entityId))
            {
                const auto& position = positions.Get(entityId).Position;
                ImGui::Text("Position: %.1f, %.1f", position.x, position.y);
            }
            else
            {
                ImGui::Text("Position: none");
            }

            if (ImGui::TreeNode("Components"))
            {
                for (const auto& storage : componentStorages)
                {
                    if (storage->Has(entityId))
                        ImGui::BulletText("%s", storage->TypeName());
                }
                ImGui::TreePop();
            }

            ImGui::TreePop();
        }
        ImGui::PopID();
    }
}
