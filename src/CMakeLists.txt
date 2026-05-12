set(TARGET_NAME main)
set(SOURCES main.cpp
        Sample/Systems/MovementSystem.cpp
        Sample/Systems/MovementSystem.h
        Sample/Systems/InitSystem.cpp
        Sample/Systems/InitSystem.h
        Sample/Components/PositionComponent.h
        Sample/Components/MovementComponent.h
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
