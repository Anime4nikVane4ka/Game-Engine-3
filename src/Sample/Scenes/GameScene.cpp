#include "GameScene.h"

#include <memory>

#include <SFML/Graphics/Color.hpp>

#include "../../GameEngine/GameEngine.h"
#include "../../GameEngine/Assets/AssetNames.h"
#include "../Components/AnimationStateComponent.h"
#include "../Components/AnimatorComponent.h"
#include "../Components/BoxColliderComponent.h"
#include "../Components/BrickTileComponent.h"
#include "../Components/CameraComponent.h"
#include "../Components/CollisionComponent.h"
#include "../Components/FinishComponent.h"
#include "../Components/DecorComponent.h"
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

GameScene::GameScene(GameEngine& gameEngine)
    : Scene(gameEngine)
{
}

void GameScene::Init()
{
    RegisterAction(sf::Keyboard::Key::A, "MoveLeft");
    RegisterAction(sf::Keyboard::Key::D, "MoveRight");
    RegisterAction(sf::Keyboard::Key::W, "Jump");
    RegisterAction(sf::Keyboard::Key::Space, "Shoot");

    const auto& explosionAnimation = gameEngine.Assets().GetAnimation(ExplosionAnim);

    systemsManager.AddSystem(std::make_shared<InputSystem>(
        world,
        actionMap["MoveLeft"],
        actionMap["MoveRight"],
        actionMap["Jump"],
        actionMap["Shoot"]));
    systemsManager.AddSystem(std::make_shared<AnimationStateSystem>(world));
    systemsManager.AddSystem(std::make_shared<GravitySystem>(world));
    systemsManager.AddSystem(std::make_shared<MovementSystem>(world));
    systemsManager.AddSystem(std::make_shared<CollisionDetectionSystem>(world));
    systemsManager.AddSystem(std::make_shared<CollisionHandlerSystem>(world, explosionAnimation));
    systemsManager.AddSystem(std::make_shared<ShootSystem>(
        world,
        gameEngine.Assets().GetTexture(Bullet),
        8.0f,
        8.0f));
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
    auto& followCameras = world.GetStorage<FollowXCameraComponent>();
    auto& gravity = world.GetStorage<GravityComponent>();
    auto& movements = world.GetStorage<MovementComponent>();
    auto& players = world.GetStorage<PlayerComponent>();
    auto& positions = world.GetStorage<PositionComponent>();
    auto& shooters = world.GetStorage<ShooterComponent>();
    auto& sprites = world.GetStorage<SpriteComponent>();
    auto& tiles = world.GetStorage<TileComponent>();

    const auto& idleAnimation = gameEngine.Assets().GetAnimation(IdleAnim);
    const auto& runAnimation = gameEngine.Assets().GetAnimation(RunAnim);
    const auto& jumpAnimation = gameEngine.Assets().GetAnimation(JumpAnim);
    const auto& shootIdleAnimation = gameEngine.Assets().GetAnimation(ShootIdleAnim);
    const auto& shootRunAnimation = gameEngine.Assets().GetAnimation(ShootRunAnim);
    const auto& shootJumpAnimation = gameEngine.Assets().GetAnimation(ShootJumpAnim);

    const int player = world.CreateEntity();
    auto& playerPosition = positions.Add(player, PositionComponent(120.0f, 480.0f));
    playerPosition.Scale = {4.0f, 4.0f};
    movements.Add(player, MovementComponent(5.0f, {0.0f, 0.0f}, 6.0f, 4.0f));
    gravity.Add(player, GravityComponent(0.100f));
    players.Add(player, PlayerComponent());
    shooters.Add(player, ShooterComponent(500.0f));
    sprites.Add(player, SpriteComponent(idleAnimation.GetTexture()));
    animationStates.Add(player, AnimationStateComponent("Idle"));
    auto& playerAnimator = animators.Add(player, AnimatorComponent());
    playerAnimator.Animations.emplace("Idle", idleAnimation);
    playerAnimator.Animations.emplace("Run", runAnimation);
    playerAnimator.Animations.emplace("Jump", jumpAnimation);
    playerAnimator.Animations.emplace("ShootIdle", shootIdleAnimation);
    playerAnimator.Animations.emplace("ShootRun", shootRunAnimation);
    playerAnimator.Animations.emplace("ShootJump", shootJumpAnimation);
    boxColliders.Add(player, BoxColliderComponent(96.0f, 96.0f));
    collisions.Add(player, CollisionComponent());

    const auto& tileTexture = gameEngine.Assets().GetTexture(Tile);
    for (int i = 0; i < 100; i++)
    {
        const int tile = world.CreateEntity();
        positions.Add(tile, PositionComponent(32.0f + i * 64.0f, 560.0f));
        sprites.Add(tile, SpriteComponent(tileTexture));
        boxColliders.Add(tile, BoxColliderComponent(64.0f, 64.0f));
        collisions.Add(tile, CollisionComponent());
        tiles.Add(tile, TileComponent());
    }

    const int brick = world.CreateEntity();
    positions.Add(brick, PositionComponent(560.0f, 400.0f));
    sprites.Add(brick, SpriteComponent(gameEngine.Assets().GetTexture(BrickTile)));
    boxColliders.Add(brick, BoxColliderComponent(64.0f, 64.0f));
    collisions.Add(brick, CollisionComponent());
    bricks.Add(brick, BrickTileComponent());

    const int finish = world.CreateEntity();
    positions.Add(finish, PositionComponent(820.0f, 496.0f));
    sprites.Add(finish, SpriteComponent(gameEngine.Assets().GetTexture(Finish)));
    boxColliders.Add(finish, BoxColliderComponent(64.0f, 128.0f));
    collisions.Add(finish, CollisionComponent());
    decorations.Add(finish, DecorComponent());

    const int camera = world.CreateEntity();
    cameras.Add(camera, CameraComponent());
    followCameras.Add(camera, FollowXCameraComponent());
}

void GameScene::Update(float delta)
{
    (void)delta;
    gameEngine.Window().clear(sf::Color(100, 100, 255));
    systemsManager.Update();
}
