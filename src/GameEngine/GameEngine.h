#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#include <memory>
#include <type_traits>
#include <unordered_map>
#include <utility>

#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/System/Clock.hpp"

#include "Assets/AssetManager.h"
#include "GameEngineConfiguration.h"
#include "Input/InputManager.h"
#include "Scene.h"

class GameEngine {
    const GameEngineConfiguration _config;

    sf::RenderWindow _window;
    sf::Clock _deltaClock;

    std::unordered_map<size_t, std::shared_ptr<Scene>> _scenes;
    AssetManager _assetManager;
    std::shared_ptr<InputManager> _inputManager;

    bool _isRunning;

    size_t _currentScene;

    void Initialize();

    template <typename T, typename... Args> typename std::enable_if<std::is_base_of<Scene, T>::value, std::shared_ptr<T>>::type CreateScene(Args&&... args) {
        std::shared_ptr<T> scene = std::make_shared<T>(std::forward<Args>(args)...);
        scene->Init();

        return scene;
    }

    template <typename T, typename... Args> typename std::enable_if<std::is_base_of<Scene, T>::value, void>::type ChangeSceneWithFlag(const bool additive, Args&&... args) {
        const auto typeHash = typeid(T).hash_code();
        _currentScene = typeHash;

        if (!additive) {
            _scenes.clear();
            auto scene = CreateScene<T>(std::forward<Args>(args)...);
            _scenes.insert({typeHash, scene});
        } else {
            const auto foundStorageIterator = _scenes.find(typeHash);
            if (foundStorageIterator == _scenes.end()) {
                auto scene = CreateScene<T>(std::forward<Args>(args)...);
                _scenes.insert({typeHash, scene});
            }
        }
    }

    void Render();

  public:
    GameEngine(const GameEngineConfiguration& config);

    std::shared_ptr<Scene> CurrentScene() {
        return _scenes[_currentScene];
    }
    const AssetManager& Assets() const {
        return _assetManager;
    }
    sf::RenderWindow& Window() {
        return _window;
    }

    template <typename T> typename std::enable_if<std::is_base_of<Scene, T>::value, void>::type ChangeScene() {
        const auto typeHash = typeid(T).hash_code();
        _currentScene = typeHash;
    }

    template <typename T, typename Arg, typename... Args> typename std::enable_if<std::is_base_of<Scene, T>::value, void>::type ChangeScene(Arg&& arg, Args&&... args) {
        ChangeSceneWithFlag<T>(true, std::forward<Arg>(arg), std::forward<Args>(args)...);
    }

    template <typename T, typename... Args> typename std::enable_if<std::is_base_of<Scene, T>::value, void>::type LoadScene(Args&&... args) {
        ChangeSceneWithFlag<T>(false, std::forward<Args>(args)...);
    }

    void Run();
    void Quit();

    template <typename T>
    typename std::enable_if<std::is_same<sf::Keyboard::Key, T>::value || std::is_same<sf::Mouse::Button, T>::value || std::is_same<sf::Mouse::Wheel, T>::value || std::is_same<MouseMove, T>::value,
        void>::type
    RegisterInput(const T type, std::shared_ptr<InputAction> action) const {
        if (_inputManager == nullptr) {
            return;
        }

        _inputManager->RegisterInput(_currentScene, type, std::move(action));
    }
};

#endif // GAMEENGINE_H
