#include "World.h"

void World::EntityComponentsChanged(const int e, const int storageId, const bool added)
{
    // ToDo: Логика обновления маски на сущности при изменении набора компонент на ней
    auto &entity = _entities[e];
    if (added)
    {
        entity.AddComponent(storageId);
    } else
    {
        const int newComponentsCount = entity.RemoveComponent(storageId);
        if (newComponentsCount == 0)
        {
            entity.Remove();
            _freeEntities.push_back(entity.Id);
        }
    }
}

World::World()
{
    _entities.reserve(DefaultEntitiesCapacity);
    _freeEntities.reserve(DefaultEntitiesCapacity);

    // ToDo: Инициализация мира и выделения памяти
}

int World::CreateEntity()
{
    // ToDo: Логика создания новой сущности или переиспользования свободной сущности
    int entityId;
    if (!_freeEntities.empty())
    {
        entityId = _freeEntities.back();
        _freeEntities.pop_back();
        _entities[entityId].Recycle();
    } else
    {
        entityId = _entities.size();
        _entities.emplace_back(entityId, 1);
    }
    return entityId;
}

void World::RemoveEntity(int ent)
{
    // ToDo: Логика удаления сущности из мира и вычистки ее из хранилищ компонентов
    auto &entity = _entities[ent];
    if (entity.IsRemoved())
        return;
    const auto components = entity.Components();
    if (!components.empty())
    {
        for (int i = static_cast<int>(components.size()) - 1; i >= 0; i--)
            _componentStorages[components[i]]->Remove(ent);
    }
    else
    {
        entity.Remove();
        _freeEntities.push_back(entity.Id);
    }
}

EntityId World::GetPackedEntity(const int e) const
{
    // ToDo: Логика получения упаковонной сущности для хранения в мире
    return _entities[e];
}

bool World::UnpackEntity(const EntityId &eId, int &e) const
{
    // ToDo: Логика распаковки и валидации сущности
    e = eId.Id;
    return IsEntityAlive(e) && eId.Gen() == _entities[e].Gen();
}

bool World::IsEntityAlive(const int e) const
{
    // ToDo: Логика проверки жива сущность и корректна или нет
    if (e < 0 || e >= _entities.size())
        return false;
    auto &entity = _entities[e];
    return !entity.IsRemoved() && e == entity.Id;
}