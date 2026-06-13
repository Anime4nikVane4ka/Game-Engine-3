#include "GameScene.h"

#include <memory>

#include <SFML/Graphics/Color.hpp>

#include "../../Config/Config.h"
#include "../../Config/LevelConfig.h"
#include "../../GameEngine/GameEngine.h"
#include "../Components/AnimationStateComponent.h"
#include "../Components/AnimatorComponent.h"
#include "../Components/BoxColliderComponent.h"
#include "../Components/BrickTileComponent.h"
#include "../Components/CameraComponent.h"
#include "../Components/CollisionComponent.h"
#include "../Components/DecorComponent.h"
#include "../Components/FinishComponent.h"
#include "../Components/FollowXCameraComponent.h"
#include "../Components/GravityComponent.h"
#include "../Components/MovementComponent.h"
#include "../Components/PlayerComponent.h"
#include "../Components/PositionComponent.h"
#include "../Components/ShooterComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Components/TileComponent.h"
#include "../Systems/AnimationStateSystem.h"
#include "../Systems/AnimationSystem.h"
#include "../Systems/CollisionDetectionSystem.h"
#include "../Systems/CollisionHandlerSystem.h"
#include "../Systems/DestroyOnAnimationEndSystem.h"
#include "../Systems/FollowCameraSystem.h"
#include "../Systems/GravitySystem.h"
#include "../Systems/InputSystem.h"
#include "../Systems/MovementSystem.h"
#include "../Systems/RenderSystem.h"
#include "../Systems/ShootSystem.h"

const char* PlayerObject = "Player";
const char* TileObject = "Tile";
const char* BrickObject = "Brick";
const char* FinishObject = "Finish";

const char* IdleState = "Idle";
const char* RunState = "Run";
const char* JumpState = "Jump";
const char* ShootIdleState = "ShootIdle";
const char* ShootRunState = "ShootRun";
const char* ShootJumpState = "ShootJump";

const DecorationConfig* FindDecoration(const std::vector<DecorationConfig>& decorations,
    const std::string& name) {
    for (const auto& decoration : decorations) {
        if (decoration.Name == name) {
            return &decoration;
        }
    }

    return nullptr;
}

GameScene::GameScene(GameEngine& gameEngine) : Scene(gameEngine) {}

void GameScene::Init() {
    RegisterAction(sf::Keyboard::Key::A, "MoveLeft");
    RegisterAction(sf::Keyboard::Key::D, "MoveRight");
    RegisterAction(sf::Keyboard::Key::W, "Jump");
    RegisterAction(sf::Keyboard::Key::Space, "Shoot");

    Config config(GameEngineConfiguration::ConfigFile);
    LevelConfig levelConfig(GameEngineConfiguration::LevelFile);

    const auto& explosionAnimation =
        gameEngine.Assets().GetAnimation(config.BrickTile.DestroyAnimation);

    systemsManager.AddSystem(std::make_shared<InputSystem>(world,
        actionMap["MoveLeft"],
        actionMap["MoveRight"],
        actionMap["Jump"],
        actionMap["Shoot"]));
    systemsManager.AddSystem(std::make_shared<AnimationStateSystem>(world));
    systemsManager.AddSystem(std::make_shared<GravitySystem>(world));
    systemsManager.AddSystem(std::make_shared<MovementSystem>(world));
    systemsManager.AddSystem(std::make_shared<CollisionDetectionSystem>(world));
    systemsManager.AddSystem(std::make_shared<CollisionHandlerSystem>(world, explosionAnimation));
    systemsManager.AddSystem(std::make_shared<ShootSystem>(world,
        gameEngine.Assets().GetTexture(config.Bullet.BaseTexture),
        config.Bullet.Radius));
    systemsManager.AddSystem(std::make_shared<FollowCameraSystem>(world, gameEngine.Window()));
    systemsManager.AddSystem(std::make_shared<AnimationSystem>(world));
    systemsManager.AddSystem(std::make_shared<DestroyOnAnimationEndSystem>(world));
    systemsManager.AddSystem(std::make_shared<RenderSystem>(world, gameEngine.Window()));

    auto& animationStates = world.GetStorage<AnimationStateComponent>();
    auto& animators = world.GetStorage<AnimatorComponent>();
    auto& boxColliders = world.GetStorage<BoxColliderComponent>();
    auto& bricks = world.GetStorage<BrickTileComponent>();
    auto& cameras = world.GetStorage<CameraComponent>();
    auto& collisions = world.GetStorage<CollisionComponent>();
    auto& decorations = world.GetStorage<DecorComponent>();
    auto& finishes = world.GetStorage<FinishComponent>();
    auto& followCameras = world.GetStorage<FollowXCameraComponent>();
    auto& gravity = world.GetStorage<GravityComponent>();
    auto& movements = world.GetStorage<MovementComponent>();
    auto& players = world.GetStorage<PlayerComponent>();
    auto& positions = world.GetStorage<PositionComponent>();
    auto& shooters = world.GetStorage<ShooterComponent>();
    auto& sprites = world.GetStorage<SpriteComponent>();
    auto& tiles = world.GetStorage<TileComponent>();

    for (const auto& object : levelConfig.Objects) {
        if (object.Name == PlayerObject) {
            const auto& baseAnimation = gameEngine.Assets().GetAnimation(config.Player.BasePose);

            const int player = world.CreateEntity();
            auto& playerPosition = positions.Add(player,
                PositionComponent(object.X + config.Player.BboxWidth / 2.0f,
                    object.Y - config.Player.BboxHeight / 2.0f));
            playerPosition.Scale = {4.0f, 4.0f};
            movements.Add(player,
                MovementComponent(config.Player.MoveSpeed,
                    {0.0f, 0.0f},
                    config.Player.MaxVelocity,
                    config.Player.JumpForce));
            gravity.Add(player, GravityComponent(config.Player.Gravity));
            players.Add(player, PlayerComponent());
            shooters.Add(player, ShooterComponent(500.0f, config.Bullet.Speed));
            sprites.Add(player, SpriteComponent(baseAnimation.GetTexture()));
            animationStates.Add(player, AnimationStateComponent(IdleState));

            auto& playerAnimator = animators.Add(player, AnimatorComponent());
            playerAnimator.Animations.emplace(IdleState,
                gameEngine.Assets().GetAnimation(config.Player.Animations.at(0)));
            playerAnimator.Animations.emplace(RunState,
                gameEngine.Assets().GetAnimation(config.Player.Animations.at(1)));
            playerAnimator.Animations.emplace(JumpState,
                gameEngine.Assets().GetAnimation(config.Player.Animations.at(2)));
            playerAnimator.Animations.emplace(ShootIdleState,
                gameEngine.Assets().GetAnimation(config.Player.Animations.at(3)));
            playerAnimator.Animations.emplace(ShootRunState,
                gameEngine.Assets().GetAnimation(config.Player.Animations.at(4)));
            playerAnimator.Animations.emplace(ShootJumpState,
                gameEngine.Assets().GetAnimation(config.Player.Animations.at(5)));

            boxColliders.Add(player,
                BoxColliderComponent(config.Player.BboxWidth, config.Player.BboxHeight));
            collisions.Add(player, CollisionComponent());

            continue;
        }

        if (object.Name == TileObject) {
            const int tile = world.CreateEntity();
            positions.Add(tile,
                PositionComponent(object.X + LevelConfig::CellSize / 2.0f,
                    object.Y - LevelConfig::CellSize / 2.0f));
            sprites.Add(tile,
                SpriteComponent(gameEngine.Assets().GetTexture(config.Tile.BaseTexture)));
            boxColliders.Add(tile,
                BoxColliderComponent(LevelConfig::CellSize, LevelConfig::CellSize));
            collisions.Add(tile, CollisionComponent());
            tiles.Add(tile, TileComponent());

            continue;
        }

        if (object.Name == BrickObject) {
            const int brick = world.CreateEntity();
            positions.Add(brick,
                PositionComponent(object.X + LevelConfig::CellSize / 2.0f,
                    object.Y - LevelConfig::CellSize / 2.0f));
            sprites.Add(brick,
                SpriteComponent(gameEngine.Assets().GetTexture(config.BrickTile.BaseTexture)));
            boxColliders.Add(brick,
                BoxColliderComponent(LevelConfig::CellSize, LevelConfig::CellSize));
            collisions.Add(brick, CollisionComponent());
            bricks.Add(brick, BrickTileComponent());

            continue;
        }

        if (object.Name == FinishObject) {
            const int finish = world.CreateEntity();
            positions.Add(finish,
                PositionComponent(object.X + LevelConfig::CellSize / 2.0f,
                    object.Y - LevelConfig::CellSize));

            const DecorationConfig* decoration = FindDecoration(config.Decorations, object.Name);
            if (decoration != nullptr)
                sprites.Add(finish,
                    SpriteComponent(gameEngine.Assets().GetTexture(decoration->BaseTexture)));

            boxColliders.Add(finish,
                BoxColliderComponent(LevelConfig::CellSize, LevelConfig::CellSize * 2.0f));
            collisions.Add(finish, CollisionComponent());
            decorations.Add(finish, DecorComponent());
            finishes.Add(finish, FinishComponent());

            continue;
        }

        const DecorationConfig* decoration = FindDecoration(config.Decorations, object.Name);
        if (decoration != nullptr) {
            const auto& texture = gameEngine.Assets().GetTexture(decoration->BaseTexture);
            const auto textureSize = texture.getSize();
            const int decor = world.CreateEntity();

            positions.Add(decor,
                PositionComponent(object.X + textureSize.x / 2.0f,
                    object.Y - textureSize.y / 2.0f));
            sprites.Add(decor, SpriteComponent(texture));
            decorations.Add(decor, DecorComponent());
        }
    }

    const int camera = world.CreateEntity();
    cameras.Add(camera, CameraComponent());
    followCameras.Add(camera, FollowXCameraComponent());
}

void GameScene::Update(float delta) {
    (void)delta;
    gameEngine.Window().clear(sf::Color(100, 100, 255));
    systemsManager.Update();
}
