#ifndef COMPONENTSTORAGE_H
#define COMPONENTSTORAGE_H

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
    }

public:
    // ToDo:
    ComponentStorage(internal::IWorldInternal& w, const int id);

    bool Has(const int e) const override
    {
        // ToDo: Проверка наличия компонента на сущности
    }

    T& Get(const int e)
    {
        // ToDo: Получение компонента с сущности
    }

    T& Add(const int e, const T& value)
    {
        // ToDo: Проверка необходимости ресайза

        // ToDo: Добавление компонента на сущность

        // ToDo: Уведомление мира об изменении набора компонентов на сущности
    }

    void Remove(const int e) override
    {
        // ToDo: FastRemove компонента с сущности

        // ToDo: Уведомление мира об изменении набора компонентов на сущности
    }

    // ToDo: возврат всех компонентов данного типа
    std::span<const T> All() const;
    // ToDo: возврат всех сущностей с компонентом данного типа
    std::span<const int> Entities() const override;

    // ToDo:
    int Count() const override;
    int Id() const override;
};

#endif //COMPONENTSTORAGE_H
