#include "CollisionHandlerSystem.h"

#include "../Components/FinishReachedEvent.h"
#include <algorithm>
#include <cmath>

#include <SFML/Graphics/Rect.hpp>

constexpr const char* ExplosionState = "Explosion";
constexpr const char* CoinState = "Coin";
constexpr int CoinLifetimeFrames = 30;
constexpr float CoinOffsetY = 64.0f;

void CollisionHandlerSystem::OnInit() {}

bool CollisionHandlerSystem::IsSolid(const int entity) const {
    return _tiles.Has(entity) || _bricks.Has(entity);
}

bool CollisionHandlerSystem::IsVerticalCollision(const int firstEntity,
    const int secondEntity) const {
    if (!_positions.Has(firstEntity) || !_positions.Has(secondEntity) ||
        !_boxColliders.Has(firstEntity) || !_boxColliders.Has(secondEntity))
        return false;

    const auto& firstPosition = _positions.Get(firstEntity);
    const auto& secondPosition = _positions.Get(secondEntity);
    const auto& firstBox = _boxColliders.Get(firstEntity);
    const auto& secondBox = _boxColliders.Get(secondEntity);

    const float overlapX = firstBox.Extents.x + secondBox.Extents.x -
                           std::abs(firstPosition.Position.x - secondPosition.Position.x);
    const float overlapY = firstBox.Extents.y + secondBox.Extents.y -
                           std::abs(firstPosition.Position.y - secondPosition.Position.y);

    return overlapY <= overlapX;
}

bool CollisionHandlerSystem::IsAbove(const int firstEntity, const int secondEntity) const {
    if (!_positions.Has(firstEntity) || !_positions.Has(secondEntity))
        return false;

    return _positions.Get(firstEntity).Position.y < _positions.Get(secondEntity).Position.y;
}

void CollisionHandlerSystem::CreateExplosion(const int brickEntity) {
    const auto& brickPosition = _positions.Get(brickEntity);
    const int explosionEntity = world.CreateEntity();

    auto& explosionPosition = _positions.Add(explosionEntity,
        PositionComponent(brickPosition.Position.x, brickPosition.Position.y));
    explosionPosition.Scale = {2.0f, 2.0f};
    auto& sprite = _sprites.Add(explosionEntity, SpriteComponent(_explosionAnimation.GetTexture()));
    sprite.TextureRect = sf::IntRect({0, 0}, _explosionAnimation.Size());
    _animationStates.Add(explosionEntity, AnimationStateComponent(ExplosionState));
    auto& animator = _animators.Add(explosionEntity, AnimatorComponent());
    animator.Animations.emplace(ExplosionState, _explosionAnimation);
    _destroyOnAnimationEnds.Add(explosionEntity, DestroyOnAnimationEndComponent());
}

void CollisionHandlerSystem::CreateCoin(const int questionTileEntity) {
    const auto& questionTilePosition = _positions.Get(questionTileEntity);
    const int coinEntity = world.CreateEntity();

    _positions.Add(coinEntity,
        PositionComponent(questionTilePosition.Position.x,
            questionTilePosition.Position.y - CoinOffsetY));
    auto& sprite = _sprites.Add(coinEntity, SpriteComponent(_coinAnimation.GetTexture()));
    sprite.TextureRect = sf::IntRect({0, 0}, _coinAnimation.Size());
    _animationStates.Add(coinEntity, AnimationStateComponent(CoinState));
    auto& animator = _animators.Add(coinEntity, AnimatorComponent());
    animator.Animations.emplace(CoinState, _coinAnimation);
    _destroyOnAnimationEnds.Add(coinEntity, DestroyOnAnimationEndComponent(CoinLifetimeFrames));
}

void CollisionHandlerSystem::ActivateQuestionTile(const int playerEntity,
    const int questionTileEntity) {
    if (!_questionTiles.Has(questionTileEntity))
        return;

    auto& questionTile = _questionTiles.Get(questionTileEntity);
    if (!questionTile.IsActive)
        return;

    const auto& movement = _movements.Get(playerEntity);
    if (movement.Direction.y >= 0.0f || IsAbove(playerEntity, questionTileEntity) ||
        !IsVerticalCollision(playerEntity, questionTileEntity))
        return;

    questionTile.IsActive = false;
    _sprites.Get(questionTileEntity) = SpriteComponent(_inactiveQuestionTexture);
    _players.Get(playerEntity).AddScore(1);
    CreateCoin(questionTileEntity);
}

void CollisionHandlerSystem::DestroyBrick(const int brickEntity,
    std::vector<int>& entitiesToRemove) {
    if (!_bricks.Has(brickEntity))
        return;

    CreateExplosion(brickEntity);
    entitiesToRemove.push_back(brickEntity);
}

void CollisionHandlerSystem::RequestPlayerRespawn(const int playerEntity) {
    if (!_respawns.Has(playerEntity))
        return;

    _respawns.Get(playerEntity).NeedRespawn = true;
}

void CollisionHandlerSystem::HandleSolidCollision(const int entity,
    const int collidedEntity,
    std::vector<int>& entitiesToRemove,
    const bool destroyBrickFromBelow) {
    if (!IsSolid(collidedEntity) || !_movements.Has(entity))
        return;

    auto& movement = _movements.Get(entity);
    if (movement.Direction.y > 0.0f && IsAbove(entity, collidedEntity) &&
        IsVerticalCollision(entity, collidedEntity)) {
        movement.Direction.y = 0.0f;
        movement.IsGrounded = true;
        if (_boxColliders.Has(entity) && _boxColliders.Has(collidedEntity)) {
            auto& entityPosition = _positions.Get(entity);
            const auto& collidedPosition = _positions.Get(collidedEntity);
            const auto& entityBox = _boxColliders.Get(entity);
            const auto& collidedBox = _boxColliders.Get(collidedEntity);
            entityPosition.Position.y =
                collidedPosition.Position.y - entityBox.Extents.y - collidedBox.Extents.y;
        }
        return;
    }

    if (movement.Direction.y < 0.0f && !IsAbove(entity, collidedEntity) &&
        IsVerticalCollision(entity, collidedEntity)) {
        movement.Direction.y = 0.0f;
        if (_boxColliders.Has(entity) && _boxColliders.Has(collidedEntity)) {
            auto& entityPosition = _positions.Get(entity);
            const auto& collidedPosition = _positions.Get(collidedEntity);
            const auto& entityBox = _boxColliders.Get(entity);
            const auto& collidedBox = _boxColliders.Get(collidedEntity);
            entityPosition.Position.y =
                collidedPosition.Position.y + entityBox.Extents.y + collidedBox.Extents.y;
        }
        if (destroyBrickFromBelow && _bricks.Has(collidedEntity))
            DestroyBrick(collidedEntity, entitiesToRemove);
    }

    if (!IsVerticalCollision(entity, collidedEntity) && movement.Direction.x != 0.0f &&
        _boxColliders.Has(entity) && _boxColliders.Has(collidedEntity)) {
        movement.Direction.x = 0.0f;

        auto& entityPosition = _positions.Get(entity);
        const auto& collidedPosition = _positions.Get(collidedEntity);
        const auto& entityBox = _boxColliders.Get(entity);
        const auto& collidedBox = _boxColliders.Get(collidedEntity);

        if (entityPosition.Position.x < collidedPosition.Position.x) {
            entityPosition.Position.x =
                collidedPosition.Position.x - entityBox.Extents.x - collidedBox.Extents.x;
        } else {
            entityPosition.Position.x =
                collidedPosition.Position.x + entityBox.Extents.x + collidedBox.Extents.x;
        }
    }
}

void CollisionHandlerSystem::HandlePlayerCollision(const int playerEntity,
    const int collidedEntity,
    std::vector<int>& entitiesToRemove) {
    if (_finishes.Has(collidedEntity)) {
        const int eventEntity = world.CreateEntity();
        world.GetStorage<FinishReachedEvent>().Add(eventEntity, FinishReachedEvent());
        if (_movements.Has(playerEntity))
            _movements.Get(playerEntity).Direction = {0.0f, 0.0f};
        return;
    }

    if (_goombas.Has(collidedEntity)) {
        RequestPlayerRespawn(playerEntity);
        return;
    }

    HandleSolidCollision(playerEntity, collidedEntity, entitiesToRemove, true);
}

void CollisionHandlerSystem::HandleGoombaCollision(const int goombaEntity,
    const int collidedEntity,
    std::vector<int>& entitiesToRemove) {
    HandleSolidCollision(goombaEntity, collidedEntity, entitiesToRemove, false);
}

void CollisionHandlerSystem::HandleBulletCollision(const int bulletEntity,
    const int collidedEntity,
    std::vector<int>& entitiesToRemove) {
    if (!_bricks.Has(collidedEntity) && !_goombas.Has(collidedEntity))
        return;

    if (std::find(entitiesToRemove.begin(), entitiesToRemove.end(), bulletEntity) ==
        entitiesToRemove.end())
        entitiesToRemove.push_back(bulletEntity);

    if (_goombas.Has(collidedEntity)) {
        if (std::find(entitiesToRemove.begin(), entitiesToRemove.end(), collidedEntity) ==
            entitiesToRemove.end())
            entitiesToRemove.push_back(collidedEntity);
        return;
    }

    DestroyBrick(collidedEntity, entitiesToRemove);
}

void CollisionHandlerSystem::OnUpdate() {
    std::vector<int> entitiesToRemove;

    for (const int entity : _collidableEntities) {
        if ((_players.Has(entity) || _goombas.Has(entity)) && _movements.Has(entity))
            _movements.Get(entity).IsGrounded = false;

        const auto& collision = _collisions.Get(entity);
        if (_players.Has(entity)) {
            for (const int collidedEntity : collision.CollidedEntities)
                ActivateQuestionTile(entity, collidedEntity);
        }

        for (const int collidedEntity : collision.CollidedEntities) {
            if (_players.Has(entity))
                HandlePlayerCollision(entity, collidedEntity, entitiesToRemove);

            if (_goombas.Has(entity))
                HandleGoombaCollision(entity, collidedEntity, entitiesToRemove);

            if (_bullets.Has(entity))
                HandleBulletCollision(entity, collidedEntity, entitiesToRemove);
        }
    }

    for (const int entity : entitiesToRemove) {
        if (world.IsEntityAlive(entity))
            world.RemoveEntity(entity);
    }
}
