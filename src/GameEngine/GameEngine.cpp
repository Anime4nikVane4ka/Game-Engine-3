#include "GameEngine.h"

#include <SFML/System/Time.hpp>
#include <imgui-SFML.h>
#include <imgui.h>

//#include "../Sample/Scenes/GameScene.h"
#include "../Sample/Scenes/MenuScene.h"

void GameEngine::Render() {
    // ToDo: ������ ������� ���� � ui
    ImGui::SFML::Render(_window);
    _window.display();
}

GameEngine::GameEngine(const GameEngineConfiguration& config)
    : _config(config),
      _window(sf::VideoMode({GameEngineConfiguration::Width, GameEngineConfiguration::Height}),
          "Game"),
      _inputManager(std::make_shared<InputManager>(_window, *this)), _isRunning(false),
      _currentScene(0) {
    _window.setFramerateLimit(60);

    ImGui::SFML::Init(_window);
    _assetManager.LoadFromFile(GameEngineConfiguration::AssetsFile);
}

void GameEngine::Initialize() {
    if (_scenes.empty()) {
        LoadScene<MenuScene>(*this);
        //LoadScene<GameScene>(*this);
    }

    _isRunning = true;
}

void GameEngine::Run() {
    Initialize();

    while (_isRunning && _window.isOpen()) {
        if (_inputManager != nullptr) {
            _isRunning = _inputManager->ProcessInput(_currentScene);
        }

        const sf::Time deltaTime = _deltaClock.restart();
        ImGui::SFML::Update(_window, deltaTime);

        _window.clear();

        const auto currentSceneIterator = _scenes.find(_currentScene);
        if (currentSceneIterator != _scenes.end() && currentSceneIterator->second != nullptr) {
            currentSceneIterator->second->Update(deltaTime.asSeconds());
        }

        Render();
    }

    ImGui::SFML::Shutdown();
}

void GameEngine::Quit() {
    _isRunning = false;
    _window.close();
}
