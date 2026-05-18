#ifndef COMPONENTSTORAGE_H
#define COMPONENTSTORAGE_H

#include <algorithm>
#include <stdexcept>
#include <typeinfo>
#include <utility>
#include <vector>

#include "BaseComponentStorage.h"
#include "../World/IWorldInternal.h"

template <typename T>
class ComponentStorage final : public BaseComponentStorage {
    internal::IWorldInternal& _world;

    std::vector<T> _data;      // Для плотного хранения данных
    std::vector<int> _sparse;  // Для разреженного хранения индексов элементов (сущностей)
    std::vector<int> _dense;   // Для плотного хранения реально существующих элементов (сущностей, на которых есть компоненты)
    
    int _count = 0;            // Текущее число элементов

    int _id;

    void Resize(const int sparseSize, const int dataSize)
    {
        // ToDo: перевыделение памяти под вектора
        int oldSparseSize = _sparse.size();
        if (oldSparseSize < sparseSize)
        {
            _sparse.resize(sparseSize);
            std::fill_n(_sparse.data() + oldSparseSize, sparseSize - oldSparseSize, -1);
        }
        if (_dense.capacity() < dataSize)
        {
            _dense.reserve(dataSize);
            _data.reserve(dataSize);
        }
    }

public:
    // ToDo:
    ComponentStorage(internal::IWorldInternal& w, const int id): _world(w), _id(id) {}

    bool Has(const int entityIid) const override
    {
        // ToDo: Проверка наличия компонента на сущности
        return entityIid >= 0 && entityIid < _sparse.size() && _sparse[entityIid] >= 0;
    }

    T& Get(const int entityIid)
    {
        // ToDo: Получение компонента с сущности
        return _data[_sparse[entityIid]];
    }

    T& Add(const int entityIid, const T& value)
    {
        // ToDo: Проверка необходимости ресайза
        Resize((entityIid / 64 + 1) * 64, _count >= _data.size() ? _data.size() + 64 : _data.size());
        // ToDo: Добавление компонента на сущность
        _data.push_back(value);
        _dense.push_back(entityIid);
        _sparse[entityIid] = _count;
        _count++;
        // ToDo: Уведомление мира об изменении набора компонентов на сущности
        _world.EntityComponentsChanged(entityIid, _id, true);
        return _data[_sparse[entityIid]];
    }

    void Remove(const int entityIid) override
    {
#if DEBUG
        if (!_world.IsEntityAlive(entityIid))
            throw std::runtime_error("Try to remove from dead entity!");
#endif
        // ToDo: FastRemove компонента с сущности
        int arrayIndex = _sparse[entityIid];
        int lastIndex = _count - 1;
        int lastEntityIid = _dense[lastIndex];

        _data[arrayIndex] = std::move(_data[lastIndex]);
        _dense[arrayIndex] = lastEntityIid;
        _sparse[lastEntityIid] = arrayIndex;
        _sparse[entityIid] = -1;

        _data.pop_back();
        _dense.pop_back();
        _count--;
// ToDo: Уведомление мира об изменении набора компонентов на сущности
        _world.EntityComponentsChanged(entityIid, _id, false);
    }

    // ToDo: возврат всех компонентов данного типа
    std::span<const T> All() const
    {
        return std::span(_data.begin(), _count);
    }
    // ToDo: возврат всех сущностей с компонентом данного типа
    std::span<const int> Entities() const override
    {
        return std::span(_dense.begin(), _count);
    }

    // ToDo:
    int Count() const override 
    { 
        return _count; 
    }

    int Id() const override 
    { 
       return _id; 
    }

    const char* TypeName() const override
    {
        return typeid(T).name();
    }
};

#endif //COMPONENTSTORAGE_H
