#ifndef WORLD_H
#define WORLD_H

#include <unordered_map>
#include <vector>
#include <memory>

#include "IWorldInternal.h"
#include "../Entity/EntityId.h"
#include "../Components/ComponentStorage.hpp"

class World final : public internal::IWorldInternal {
    const int DefaultEntitiesCapacity = 64;

    std::vector<EntityId> _entities;
    std::vector<int> _freeEntities;

    std::unordered_map<size_t, std::shared_ptr<BaseComponentStorage>> _componentStoragesHash;
    std::vector<std::shared_ptr<BaseComponentStorage>> _componentStorages;

    int _storagesCount = 0;

public:
    World();

    int CreateEntity();
    void RemoveEntity(int e);

    EntityId GetPackedEntity(int e) const;
    bool UnpackEntity(const EntityId& eId, int& e) const;

    bool IsEntityAlive(int e) const override;
    void EntityComponentsChanged(int e, int storageId, bool added) override;

    template <typename T>
    std::shared_ptr<ComponentStorage<T>> GetRawStorage()
    {
        // ToDo: Логика получения указателя на хранилище по его типу из мапы
        // или создания хранилища, если его еще нет
    }

    template <typename T>
    ComponentStorage<T>& GetStorage()
    {
        // ToDo: Логика получения ссылки на хранилище по его типу из мапы
        // или создания хранилища, если его еще нет
    }
};

#endif //WORLD_H
