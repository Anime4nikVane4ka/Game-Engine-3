#include "CollisionHandlerSystem.h"

#include <algorithm>
#include <cmath>
#include "../Components/FinishReachedEvent.h"

#include <SFML/Graphics/Rect.hpp>

constexpr const char* ExplosionState = "Explosion";

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

void CollisionHandlerSystem::DestroyBrick(const int brickEntity,
    std::vector<int>& entitiesToRemove) {
    if (!_bricks.Has(brickEntity))
        return;

    CreateExplosion(brickEntity);
    entitiesToRemove.push_back(brickEntity);
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

    if (!IsSolid(collidedEntity) || !_movements.Has(playerEntity))
        return;

    auto& movement = _movements.Get(playerEntity);
    if (movement.Direction.y > 0.0f && IsAbove(playerEntity, collidedEntity) &&
        IsVerticalCollision(playerEntity, collidedEntity)) {
        movement.Direction.y = 0.0f;
        movement.IsGrounded = true;
        if (_boxColliders.Has(playerEntity) && _boxColliders.Has(collidedEntity)) {
            auto& playerPosition = _positions.Get(playerEntity);
            const auto& collidedPosition = _positions.Get(collidedEntity);
            const auto& playerBox = _boxColliders.Get(playerEntity);
            const auto& collidedBox = _boxColliders.Get(collidedEntity);
            playerPosition.Position.y =
                collidedPosition.Position.y - playerBox.Extents.y - collidedBox.Extents.y;
        }
        return;
    }

    if (movement.Direction.y < 0.0f && !IsAbove(playerEntity, collidedEntity) &&
        IsVerticalCollision(playerEntity, collidedEntity)) {
        movement.Direction.y = 0.0f;
        if (_boxColliders.Has(playerEntity) && _boxColliders.Has(collidedEntity)) {
            auto& playerPosition = _positions.Get(playerEntity);
            const auto& collidedPosition = _positions.Get(collidedEntity);
            const auto& playerBox = _boxColliders.Get(playerEntity);
            const auto& collidedBox = _boxColliders.Get(collidedEntity);
            playerPosition.Position.y =
                collidedPosition.Position.y + playerBox.Extents.y + collidedBox.Extents.y;
        }
        if (_bricks.Has(collidedEntity))
            DestroyBrick(collidedEntity, entitiesToRemove);
    }

    if (!IsVerticalCollision(playerEntity, collidedEntity) && movement.Direction.x != 0.0f &&
        _boxColliders.Has(playerEntity) && _boxColliders.Has(collidedEntity)) {
        movement.Direction.x = 0.0f;

        auto& playerPosition = _positions.Get(playerEntity);
        const auto& collidedPosition = _positions.Get(collidedEntity);
        const auto& playerBox = _boxColliders.Get(playerEntity);
        const auto& collidedBox = _boxColliders.Get(collidedEntity);

        if (playerPosition.Position.x < collidedPosition.Position.x) {
            playerPosition.Position.x =
                collidedPosition.Position.x - playerBox.Extents.x - collidedBox.Extents.x;
        } else {
            playerPosition.Position.x =
                collidedPosition.Position.x + playerBox.Extents.x + collidedBox.Extents.x;
        }
    }
}

void CollisionHandlerSystem::HandleBulletCollision(const int bulletEntity,
    const int collidedEntity,
    std::vector<int>& entitiesToRemove) {
    if (!_bricks.Has(collidedEntity))
        return;

    if (std::find(entitiesToRemove.begin(), entitiesToRemove.end(), bulletEntity) ==
        entitiesToRemove.end())
        entitiesToRemove.push_back(bulletEntity);
    DestroyBrick(collidedEntity, entitiesToRemove);
}

void CollisionHandlerSystem::OnUpdate() {
    std::vector<int> entitiesToRemove;

    for (const int entity : _collidableEntities) {
        if (_players.Has(entity) && _movements.Has(entity))
            _movements.Get(entity).IsGrounded = false;

        const auto& collision = _collisions.Get(entity);
        for (const int collidedEntity : collision.CollidedEntities) {
            if (_players.Has(entity))
                HandlePlayerCollision(entity, collidedEntity, entitiesToRemove);

            if (_bullets.Has(entity))
                HandleBulletCollision(entity, collidedEntity, entitiesToRemove);
        }
    }

    for (const int entity : entitiesToRemove) {
        if (world.IsEntityAlive(entity))
            world.RemoveEntity(entity);
    }
}
