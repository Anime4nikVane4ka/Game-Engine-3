    #include "EditorScene.h"

#include <fstream>

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Window/Event.hpp>
#include <imgui-SFML.h>
#include <imgui.h>
#include <nlohmann/json.hpp>

#include "../../Config/LevelConfig.h"
#include "../../GameEngine/GameEngine.h"
#include "MenuScene.h"
#include <SFML/Graphics/VertexArray.hpp>
#include <cmath>

namespace {
sf::Vector2f SnapToGridCenter(const sf::Vector2f& position) {
    constexpr float cellSize = 64.0f;

    return {std::floor(position.x / cellSize) * cellSize + cellSize / 2.0f, std::floor(position.y / cellSize) * cellSize + cellSize / 2.0f};
}
} // namespace
namespace {
sf::Vector2f FitToCellScale(const sf::Texture& texture) {
    constexpr float cellSize = 64.0f;

    const sf::Vector2u size = texture.getSize();

    if (size.x == 0 || size.y == 0) {
        return {1.0f, 1.0f};
    }

    return {cellSize / static_cast<float>(size.x), cellSize / static_cast<float>(size.y)};
}
} // namespace
std::string LevelObjectNameToAssetName(const std::string& objectName) {
    if (objectName == "Player") {
        return "TexIdle";
    }

    if (objectName == "Brick") {
        return "Brick Tile";
    }

    return objectName;
}
namespace {
std::string AssetNameToLevelObjectName(const std::string& assetName) {
    if (assetName == "TexIdle" || assetName == "TexRun" || assetName == "TexJump" || assetName == "TexShootIdle" || assetName == "TexShootRun" || assetName == "TexShootJump") {
        return "Player";
    }

    if (assetName == "Tile" || assetName == "Tile1") {
        return "Tile";
    }

    if (assetName == "Brick Tile") {
        return "Brick";
    }

    if (assetName == "Question Tile") {
        return "Question";
    }
    if (assetName == "Gumba" || assetName == "Goomba" || assetName == "TexGoombaMove") {
        return "Goomba";
    }

    return assetName;
}
} // namespace

EditorScene::EditorScene(GameEngine& gameEngine) : Scene(gameEngine) {}

void EditorScene::Init() {
    _view = gameEngine.Window().getDefaultView();
    LoadLevelFromFile("level_editor.json");
    RegisterAction(sf::Mouse::Wheel::Vertical, "Zoom");
    RegisterAction(sf::Keyboard::Key::Escape, "ExitToMenu");
}

void EditorScene::DrawObjects() {
    for (const auto& object : _objects) {
        if (object.Texture == nullptr) {
            continue;
        }

        sf::Sprite sprite(*object.Texture);
        const auto textureSize = object.Texture->getSize();

        sprite.setOrigin({static_cast<float>(textureSize.x) / 2.0f, static_cast<float>(textureSize.y) / 2.0f});

        sprite.setPosition(object.Position);
        sprite.setScale(object.Scale);

        gameEngine.Window().draw(sprite);
    }
}

void EditorScene::DrawEditorUi() {
    ImGui::Begin("Level Editor");

    ImGui::Text("Assets");

    for (const auto& [name, texture] : gameEngine.Assets().GetTextures()) {
        ImGui::PushID(name.c_str());
        ImGui::ImageButton(name.c_str(), texture, sf::Vector2f(64.0f, 64.0f));
        if (ImGui::IsItemClicked(ImGuiMouseButton_Left)) {
            _selectedAssetName = name;
            _selectedTexture = &texture;
            CreateObjectInCenter();
        }

        ImGui::SameLine();
        ImGui::Text("%s", name.c_str());

        ImGui::PopID();
    }

    ImGui::Separator();

    if (ImGui::Button("Save level")) {
        SaveLevelToFile("level_editor.json");
    }
    if (_selectedObjectIndex >= 0 && _selectedObjectIndex < static_cast<int>(_objects.size())) {
        ImGui::Text("Selected: %s", _objects[_selectedObjectIndex].AssetName.c_str());
    } else {
        ImGui::Text("Selected: none");
    }
    if (ImGui::Button("Delete selected")) {
        if (_selectedObjectIndex >= 0 && _selectedObjectIndex < static_cast<int>(_objects.size())) {
            _objects.erase(_objects.begin() + _selectedObjectIndex);
            _selectedObjectIndex = -1;
            _draggedObjectIndex = -1;
        }
    }

    ImGui::End();
}

void EditorScene::LoadLevelFromFile(const std::string& path) {
    LevelConfig levelConfig(path);

    _objects.clear();

    for (const auto& levelObject : levelConfig.Objects) {
        const std::string assetName = LevelObjectNameToAssetName(levelObject.Name);

        EditorObject object;
        object.AssetName = assetName;

        try {
            object.Texture = &gameEngine.Assets().GetTexture(assetName);
        } catch (...) {
            continue;
        }

        object.Position = {static_cast<float>(levelObject.X) + LevelConfig::CellSize / 2.0f, static_cast<float>(levelObject.Y) - LevelConfig::CellSize / 2.0f};

        object.Scale = FitToCellScale(*object.Texture);

        _objects.push_back(object);
    }
}

int EditorScene::FindObjectAt(const sf::Vector2f& position) const {
    for (int i = static_cast<int>(_objects.size()) - 1; i >= 0; --i) {
        const auto& object = _objects[i];

        if (object.Texture == nullptr) {
            continue;
        }

        const auto textureSize = object.Texture->getSize();

        const sf::Vector2f size(static_cast<float>(textureSize.x) * object.Scale.x, static_cast<float>(textureSize.y) * object.Scale.y);

        const sf::FloatRect bounds({object.Position.x - size.x / 2.0f, object.Position.y - size.y / 2.0f}, size);

        if (bounds.contains(position)) {
            return i;
        }
    }

    return -1;
}

void EditorScene::CreateObjectInCenter() {
    if (_selectedTexture == nullptr) {
        return;
    }

    auto& window = gameEngine.Window();

    const sf::Vector2i mousePixel = sf::Mouse::getPosition(window);
    const sf::Vector2f mouseWorld = window.mapPixelToCoords(mousePixel, _view);

    EditorObject object;
    object.AssetName = _selectedAssetName;
    object.Texture = _selectedTexture;
    object.Position = SnapToGridCenter(mouseWorld);
    object.Scale = FitToCellScale(*_selectedTexture);

    _objects.push_back(object);

    _selectedObjectIndex = static_cast<int>(_objects.size()) - 1;
    _draggedObjectIndex = _selectedObjectIndex;

    _dragOffset = _objects[_draggedObjectIndex].Position - mouseWorld;
}
void EditorScene::HandleCameraPan() {
    auto& window = gameEngine.Window();
    if (ImGui::GetIO().WantCaptureMouse) {
        _isDraggingView = false;
        return;
    }
    const bool mousePressed = sf::Mouse::isButtonPressed(sf::Mouse::Button::Right);
    const sf::Vector2i mousePosition = sf::Mouse::getPosition(window);

    if (!mousePressed) {
        _isDraggingView = false;
        _lastMousePosition = mousePosition;
        return;
    }

    if (!_isDraggingView) {
        _isDraggingView = true;
        _lastMousePosition = mousePosition;
        return;
    }

    const sf::Vector2f previousWorldPosition = window.mapPixelToCoords(_lastMousePosition, _view);

    const sf::Vector2f currentWorldPosition = window.mapPixelToCoords(mousePosition, _view);

    const sf::Vector2f delta = previousWorldPosition - currentWorldPosition;

    _view.move(delta);

    _lastMousePosition = mousePosition;
}
void EditorScene::HandleZoom() {
    if (ImGui::GetIO().WantCaptureMouse) {
        return;
    }

    const auto zoomActionIterator = actionMap.find("Zoom");

    if (zoomActionIterator == actionMap.end() || zoomActionIterator->second == nullptr) {
        return;
    }

    const auto& zoomAction = zoomActionIterator->second;

    if (zoomAction->Type() != Start) {
        return;
    }

    const float wheelDelta = static_cast<float>(zoomAction->Value());

    if (wheelDelta > 0.0f) {
        _view.zoom(0.9f);
    }

    if (wheelDelta < 0.0f) {
        _view.zoom(1.1f);
    }
}
void EditorScene::HandleObjectDrag() {
    if (ImGui::GetIO().WantCaptureMouse && _draggedObjectIndex < 0) {
        return;
    }

    auto& window = gameEngine.Window();

    const bool mousePressed = sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);
    const sf::Vector2i mousePixel = sf::Mouse::getPosition(window);
    const sf::Vector2f mouseWorld = window.mapPixelToCoords(mousePixel, _view);

    if (!mousePressed) {
        _draggedObjectIndex = -1;
        _dragOffset = {0.0f, 0.0f};
        return;
    }

    if (_draggedObjectIndex < 0) {
        const int foundObjectIndex = FindObjectAt(mouseWorld);

        if (foundObjectIndex >= 0) {
            _selectedObjectIndex = foundObjectIndex;
            _draggedObjectIndex = foundObjectIndex;
            _dragOffset = _objects[foundObjectIndex].Position - mouseWorld;
        }
    }

    if (_draggedObjectIndex >= 0) {
        _objects[_draggedObjectIndex].Position = SnapToGridCenter(mouseWorld + _dragOffset);
    }
}
void EditorScene::SaveLevelToFile(const std::string& path) {
    nlohmann::json json;

    json["width"] = 100;
    json["height"] = 12;
    json["objects"] = nlohmann::json::array();

    for (const auto& object : _objects) {
        const std::string objectName = AssetNameToLevelObjectName(object.AssetName);

        const int gridX = static_cast<int>(std::floor(object.Position.x / LevelConfig::CellSize));
        const int gridY = 12 - static_cast<int>(std::floor(object.Position.y / LevelConfig::CellSize)) - 1;

        json["objects"].push_back({{"name", objectName}, {"x", gridX}, {"y", gridY}});
    }

    std::ofstream file(path);

    if (file.is_open()) {
        file << json.dump(2);
    }
}

void EditorScene::DrawGrid() {
    constexpr float cellSize = 64.0f;

    auto& window = gameEngine.Window();

    const sf::View view = window.getView();
    const sf::Vector2f center = view.getCenter();
    const sf::Vector2f size = view.getSize();

    const float left = center.x - size.x / 2.0f;
    const float right = center.x + size.x / 2.0f;
    const float top = center.y - size.y / 2.0f;
    const float bottom = center.y + size.y / 2.0f;

    sf::VertexArray lines(sf::PrimitiveType::Lines);

    const float startX = std::floor(left / cellSize) * cellSize;
    const float startY = std::floor(top / cellSize) * cellSize;

    for (float x = startX; x <= right; x += cellSize) {
        lines.append(sf::Vertex({x, top}, sf::Color(255, 255, 255, 70)));
        lines.append(sf::Vertex({x, bottom}, sf::Color(255, 255, 255, 70)));
    }

    for (float y = startY; y <= bottom; y += cellSize) {
        lines.append(sf::Vertex({left, y}, sf::Color(255, 255, 255, 70)));
        lines.append(sf::Vertex({right, y}, sf::Color(255, 255, 255, 70)));
    }

    window.draw(lines);

    const bool showCoordinates = size.x <= 2500.0f;

    if (showCoordinates) {
        ImDrawList* drawList = ImGui::GetBackgroundDrawList();

        for (float x = startX; x <= right; x += cellSize) {
            for (float y = startY; y <= bottom; y += cellSize) {
                const int gridX = static_cast<int>(std::floor(x / cellSize));
                const int gridY = -static_cast<int>(std::floor(y / cellSize));

                const sf::Vector2i screenPosition = window.mapCoordsToPixel({x + 4.0f, y + 4.0f}, _view);

                const std::string text = "(" + std::to_string(gridX) + ", " + std::to_string(gridY) + ")";

                drawList->AddText(ImVec2(static_cast<float>(screenPosition.x), static_cast<float>(screenPosition.y)), IM_COL32(255, 255, 255, 150), text.c_str());
            }
        }
    }
}

void EditorScene::Update(float delta) {
    (void)delta;
    if (actionMap["ExitToMenu"]->Type() == Start) {
        gameEngine.LoadScene<MenuScene>(gameEngine);
        return;
    }

    gameEngine.Window().clear(sf::Color(100, 100, 255));

    HandleCameraPan();
    HandleZoom();
    HandleObjectDrag();

    gameEngine.Window().setView(_view);
    DrawGrid();
    DrawObjects();

    gameEngine.Window().setView(gameEngine.Window().getDefaultView());
    DrawEditorUi();
}