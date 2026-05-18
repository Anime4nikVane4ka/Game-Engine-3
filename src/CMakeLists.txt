set(TARGET_NAME main)
set(SOURCES main.cpp
        Sample/Systems/MovementSystem.cpp
        Sample/Systems/MovementSystem.h
        Sample/Systems/InitSystem.cpp
        Sample/Systems/InitSystem.h
        Sample/Systems/AnimationStateSystem.cpp
        Sample/Systems/AnimationStateSystem.h
        Sample/Systems/AnimationSystem.cpp
        Sample/Systems/AnimationSystem.h
        Sample/Systems/CollisionHandlerSystem.cpp
        Sample/Systems/CollisionHandlerSystem.h
        Sample/Systems/GravitySystem.cpp
        Sample/Systems/GravitySystem.h
        Sample/Systems/ShootSystem.cpp
        Sample/Systems/ShootSystem.h
        Sample/Components/PositionComponent.h
        Sample/Components/MovementComponent.h
        Sample/Components/AnimationStateComponent.h
        Sample/Components/AnimatorComponent.h
        Sample/Components/BrickTileComponent.h
        Sample/Components/DecorComponent.h
        Sample/Components/DefaultCameraComponent.h
        Sample/Components/FinishComponent.h
        Sample/Components/FollowXCameraComponent.h
        Sample/Components/GravityComponent.h
        Sample/Components/QuestionTileComponent.h
        Sample/Components/SpriteComponent.h
        Sample/Components/TileComponent.h
        Sample/Systems/InputSystem.cpp
        Sample/Systems/InputSystem.h
        Sample/Components/MoveInputEvent.h
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
        GameEngine/Assets/Animation.cpp
        GameEngine/Assets/Animation.h
)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/Assets
        DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY assets.json
        DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/Shaders
        DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_subdirectory(Ecs)

add_executable(${TARGET_NAME}
        ${SOURCES})

#target_compile_options(${TARGET_NAME} PRIVATE -Wall -Wextra -Wpedantic -Werror)
target_compile_features(${TARGET_NAME} PRIVATE cxx_std_20)

target_link_libraries(${TARGET_NAME} PUBLIC ${ECS_LIB_NAME})
target_link_libraries(${TARGET_NAME} PUBLIC ImGui-SFML::ImGui-SFML)
target_link_libraries(${TARGET_NAME} PRIVATE nlohmann_json::nlohmann_json)
