#include "Filter.h"

#include <limits>

size_t Filter::FindMinComponentStorage() const
{
    // ToDo: Логика поиска хранилища с наименьшим колчиество сущностей
    size_t minStorageIndex = 0;
    int minCount = std::numeric_limits<int>::max();

    for (size_t i = 0; i < _componentStorages.size(); i++)
    {
        int count = _componentStorages[i]->Count();
        if (count < minCount)
        {
            minCount = count;
            minStorageIndex = i;
        }
    }

    return minStorageIndex;
}

bool Filter::Iterator::HasAllComponents() const
{
    // ToDo: Логика проверки, что сущность имеет все необходимые для фильтра компоненты
    for (size_t i = 0; i < _storages.size(); i++)
    {
        if (i == _minStorageIndex)
            continue;

        if (!_storages[i]->Has(_currentEntity))
            return false;
    }

    return true;
}

Filter::Iterator& Filter::Iterator::Increment()
{
    // ToDo: Логика взятия следующей подходящей (имеющей все необходимые компоненты)
    // сущности итератором
    while (++_current < _minStorageEntities.size())
    {
        _currentEntity = _minStorageEntities[_current];
        if (HasAllComponents())
            break;
    }
    return *this;
}

Filter::Iterator::value_type Filter::Iterator::operator*() const
{
    // ToDo: Логика возврата текущей сущности итератором
    return _currentEntity;
}

Filter::Iterator& Filter::Iterator::operator++()
{
    return Increment();
}

bool Filter::Iterator::operator!=(const Iterator &other) const
{
    return _current != other._current;
}

Filter::Filter(World& world, const std::vector<std::shared_ptr<BaseComponentStorage>>& componentStorages)
    : _world(world),
      _componentStorages(componentStorages),
      _minStorageIndex(FindMinComponentStorage())
{
}

Filter::Iterator::Iterator(World& world,
    const std::vector<std::shared_ptr<BaseComponentStorage>>& storages,
    const std::span<const int>& minStorageEntities, const size_t minStorageIndex,
    const size_t current)
    : _world(world),
      _minStorageEntities(minStorageEntities),
      _storages(storages),
      _minStorageIndex(minStorageIndex),
      _currentEntity(current < minStorageEntities.size() ? minStorageEntities[current] : -1),
      _current(current)
{
    //Чтобы в begin иттератор указывал на первую ПОДХОДЯЩУЮ сущность
    if (_current < _minStorageEntities.size() && !HasAllComponents())
        Increment(); 
}

Filter::Iterator Filter::begin()
{
    // ToDo: Ищем наименьшее по количеству сущностей хранилище компонентов
    // ToDo: создаем и возвращаем итератор по этому хранилищу, который указывает на
    // первую подходящую сущность
    _minStorageIndex = FindMinComponentStorage();
    const auto& minStorageEntities = _componentStorages[_minStorageIndex]->Entities();
    const int current = 0;
    return Iterator(_world, _componentStorages,
        minStorageEntities, _minStorageIndex, current);
}

Filter::Iterator Filter::end()
{
    // ToDo: создаем и возвращаем итератор по минимальному хранилищу, который указывает на
    // конец итерирования (current = minStorageEntities.size())
    _minStorageIndex = FindMinComponentStorage();
    const auto& minStorageEntities = _componentStorages[_minStorageIndex]->Entities();
    const int current = minStorageEntities.size();
    return Iterator(_world, _componentStorages,
        minStorageEntities, _minStorageIndex, current);
}
