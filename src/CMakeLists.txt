set(TARGET_NAME main)
set(SOURCES main.cpp
        Sample/Systems/MovementSystem.cpp
        Sample/Systems/MovementSystem.h
        Sample/Systems/RenderSystem.cpp
        Sample/Systems/RenderSystem.h
        Sample/Systems/InitSystem.cpp
        Sample/Systems/InitSystem.h
        Sample/Systems/AnimationStateSystem.cpp
        Sample/Systems/AnimationStateSystem.h
        Sample/Systems/AnimationSystem.cpp
        Sample/Systems/AnimationSystem.h
        Sample/Systems/CollisionDetectionSystem.cpp
        Sample/Systems/CollisionDetectionSystem.h
        Sample/Systems/CollisionHandlerSystem.cpp
        Sample/Systems/CollisionHandlerSystem.h
        Sample/Systems/DestroyOnAnimationEndSystem.cpp
        Sample/Systems/DestroyOnAnimationEndSystem.h
        Sample/Systems/DefaultCameraSystem.cpp
        Sample/Systems/DefaultCameraSystem.h
        Sample/Systems/FollowCameraSystem.cpp
        Sample/Systems/FollowCameraSystem.h
        Sample/Systems/GravitySystem.cpp
        Sample/Systems/GravitySystem.h
        Sample/Systems/ShootSystem.cpp
        Sample/Systems/ShootSystem.h
        Sample/Scenes/MenuScene.cpp
        Sample/Scenes/MenuScene.h
        Sample/Scenes/GameScene.cpp
        Sample/Scenes/GameScene.h
        Sample/Components/PositionComponent.h
        Sample/Components/MovementComponent.h
        Sample/Components/AnimationStateComponent.h
        Sample/Components/AnimatorComponent.h
        Sample/Components/BoxColliderComponent.h
        Sample/Components/BrickTileComponent.h
        Sample/Components/BulletComponent.h
        Sample/Components/CameraComponent.h
        Sample/Components/CircleColliderComponent.h
        Sample/Components/CollisionComponent.h
        Sample/Components/DecorComponent.h
        Sample/Components/DefaultCameraComponent.h
        Sample/Components/DestroyOnAnimationEndComponent.h
        Sample/Components/FinishComponent.h
        Sample/Components/FollowXCameraComponent.h
        Sample/Components/GravityComponent.h
        Sample/Components/PlayerComponent.h
        Sample/Components/QuestionTileComponent.h
        Sample/Components/ShooterComponent.h
        Sample/Components/SpriteComponent.h
        Sample/Components/TileComponent.h
        Sample/Systems/InputSystem.cpp
        Sample/Systems/InputSystem.h
        Sample/Components/MoveInputEvent.h
        Sample/Components/FinishReachedEvent.h
        Sample/Components/RespawnComponent.h
        Sample/Systems/RespawnSystem.cpp
        Sample/Systems/RespawnSystem.h
        GameEngine/Input/InputManager.cpp
        GameEngine/Input/InputManager.h
        GameEngine/Input/InputAction.cpp
        GameEngine/Input/InputAction.h
        GameEngine/GameEngine.cpp
        GameEngine/GameEngine.h
        GameEngine/GameEngineConfiguration.h
        GameEngine/Scene.cpp
        GameEngine/Scene.h
        GameEngine/Assets/AssetManager.cpp
        GameEngine/Assets/AssetManager.h
        GameEngine/Assets/AssetNames.h
        GameEngine/Assets/Animation.cpp
        GameEngine/Assets/Animation.h
        Config/Config.cpp
        Config/Config.h
        Config/LevelConfig.cpp
        Config/LevelConfig.h
        Sample/Components/FinishReachedEvent.h
        Sample/Components/RespawnComponent.h
        Sample/Systems/RespawnSystem.h
        Sample/Systems/RespawnSystem.cpp
        GUI/GUI.cpp
        GUI/GUI.h
        Sample/BestTimeStorage.h
        Sample/BestTimeStorage.cpp
)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/Assets
        DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY assets.json
        DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY config.json
        DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY level.json
        DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_subdirectory(Ecs)

add_executable(${TARGET_NAME}
        ${SOURCES})

#target_compile_options(${TARGET_NAME} PRIVATE -Wall -Wextra -Wpedantic -Werror)
target_compile_features(${TARGET_NAME} PRIVATE cxx_std_20)

target_link_libraries(${TARGET_NAME} PUBLIC ${ECS_LIB_NAME})
target_link_libraries(${TARGET_NAME} PUBLIC ImGui-SFML::ImGui-SFML)
target_link_libraries(${TARGET_NAME} PRIVATE nlohmann_json::nlohmann_json)
