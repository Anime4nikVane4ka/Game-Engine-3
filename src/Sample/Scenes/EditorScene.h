
#ifndef EDITORSCENE_H
#define EDITORSCENE_H

#include <string>
#include <vector>

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/View.hpp>

#include "../../GameEngine/Scene.h"

struct EditorObject {
    std::string AssetName;
    const sf::Texture* Texture = nullptr;
    sf::Vector2f Position;
    sf::Vector2f Scale = {1.0f, 1.0f};
};

class EditorScene final : public Scene {
    sf::View _view;

    std::vector<EditorObject> _objects;

    std::string _selectedAssetName;
    const sf::Texture* _selectedTexture = nullptr;

    int _selectedObjectIndex = -1;
    int _draggedObjectIndex = -1;
    sf::Vector2f _dragOffset = {0.0f, 0.0f};
    bool _isDraggingView = false;
    sf::Vector2i _lastMousePosition;

    float _zoomDelta = 0.0f;

    void DrawEditorUi();
    void DrawGrid();
    void DrawObjects();
    void HandleCameraPan();
    void HandleZoom();
    void HandleObjectDrag();
    void CreateObjectInCenter();
    void SaveLevelToFile(const std::string& path);

    void LoadLevelFromFile(const std::string& path);
    int FindObjectAt(const sf::Vector2f& position) const;

  public:
    EditorScene(GameEngine& gameEngine);

    void Init() override;
    void Update(float delta) override;
};

#endif // EDITORSCENE_H