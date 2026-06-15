#ifndef COLLISIONHANDLERSYSTEM_H
#define COLLISIONHANDLERSYSTEM_H

#include <vector>

#include "../../Ecs/Filter/Filter.h"
#include "../../Ecs/Filter/FilterBuilder.h"
#include "../../Ecs/Systems/ISystem.h"
#include "../../GameEngine/Assets/Animation.h"
#include "../Components/AnimationStateComponent.h"
#include "../Components/AnimatorComponent.h"
#include "../Components/BoxColliderComponent.h"
#include "../Components/BrickTileComponent.h"
#include "../Components/BulletComponent.h"
#include "../Components/CollisionComponent.h"
#include "../Components/DamageEventComponent.h"
#include "../Components/DestroyOnAnimationEndComponent.h"
#include "../Components/FinishComponent.h"
#include "../Components/GoombaComponent.h"
#include "../Components/MovementComponent.h"
#include "../Components/PlayerComponent.h"
#include "../Components/PositionComponent.h"
#include "../Components/QuestionTileComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Components/TileComponent.h"

class CollisionHandlerSystem final : public ISystem {
    ComponentStorage<AnimationStateComponent>& _animationStates;
    ComponentStorage<AnimatorComponent>& _animators;
    ComponentStorage<CollisionComponent>& _collisions;
    ComponentStorage<DamageEventComponent>& _damageEvents;
    ComponentStorage<BoxColliderComponent>& _boxColliders;
    ComponentStorage<BrickTileComponent>& _bricks;
    ComponentStorage<BulletComponent>& _bullets;
    ComponentStorage<DestroyOnAnimationEndComponent>& _destroyOnAnimationEnds;
    ComponentStorage<FinishComponent>& _finishes;
    ComponentStorage<GoombaComponent>& _goombas;
    ComponentStorage<MovementComponent>& _movements;
    ComponentStorage<PlayerComponent>& _players;
    ComponentStorage<PositionComponent>& _positions;
    ComponentStorage<QuestionTileComponent>& _questionTiles;
    ComponentStorage<SpriteComponent>& _sprites;
    ComponentStorage<TileComponent>& _tiles;

    Filter _collidableEntities;
    const Animation& _coinAnimation;
    const Animation& _explosionAnimation;
    const sf::Texture& _inactiveQuestionTexture;

    bool IsSolid(int entity) const;
    bool IsVerticalCollision(int firstEntity, int secondEntity) const;
    bool IsAbove(int firstEntity, int secondEntity) const;
    void CreateExplosion(int brickEntity);
    void CreateCoin(int questionTileEntity);
    void CreateDamageEvent(int firstEntity, int secondEntity);
    void ActivateQuestionTile(int playerEntity, int questionTileEntity);
    void DestroyBrick(int brickEntity, std::vector<int>& entitiesToRemove);
    void HandleSolidCollision(int entity, int collidedEntity, std::vector<int>& entitiesToRemove, bool destroyBrickFromBelow);
    void HandlePlayerCollision(int playerEntity, int collidedEntity, std::vector<int>& entitiesToRemove);
    void HandleGoombaCollision(int goombaEntity, int collidedEntity, std::vector<int>& entitiesToRemove);
    void HandleBulletCollision(int bulletEntity, int collidedEntity, std::vector<int>& entitiesToRemove);

  public:
    CollisionHandlerSystem(World& world, const Animation& explosionAnimation, const Animation& coinAnimation, const sf::Texture& inactiveQuestionTexture)
        : ISystem(world), _animationStates(world.GetStorage<AnimationStateComponent>()), _animators(world.GetStorage<AnimatorComponent>()), _collisions(world.GetStorage<CollisionComponent>()),
          _damageEvents(world.GetStorage<DamageEventComponent>()), _boxColliders(world.GetStorage<BoxColliderComponent>()), _bricks(world.GetStorage<BrickTileComponent>()), _bullets(world.GetStorage<BulletComponent>()),
          _destroyOnAnimationEnds(world.GetStorage<DestroyOnAnimationEndComponent>()), _finishes(world.GetStorage<FinishComponent>()), _goombas(world.GetStorage<GoombaComponent>()),
          _movements(world.GetStorage<MovementComponent>()), _players(world.GetStorage<PlayerComponent>()), _positions(world.GetStorage<PositionComponent>()),
          _questionTiles(world.GetStorage<QuestionTileComponent>()), _sprites(world.GetStorage<SpriteComponent>()), _tiles(world.GetStorage<TileComponent>()),
          _collidableEntities(FilterBuilder(world).With<CollisionComponent>().Build()), _coinAnimation(coinAnimation),
          _explosionAnimation(explosionAnimation), _inactiveQuestionTexture(inactiveQuestionTexture) {}

    void OnInit() override;
    void OnUpdate() override;
};

#endif // COLLISIONHANDLERSYSTEM_H
