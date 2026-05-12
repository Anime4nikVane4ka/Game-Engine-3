#include "Filter.h"

#include <limits>

size_t Filter::FindMinComponentStorage() const
{
    // ToDo: Логика поиска хранилища с наименьшим колчиество сущностей
}

bool Filter::Iterator::HasAllComponents() const
{
    // ToDo: Логика проверки, что сущность имеет все необходимые для фильтра компоненты
}

Filter::Iterator& Filter::Iterator::Increment()
{
    // ToDo: Логика взятия следующей подходящей (имеющей все необходимые компоненты)
    // сущности итератором
}

Filter::Iterator::value_type Filter::Iterator::operator*() const
{
    // ToDo: Логика возврата текущей сущности итератором
}

Filter::Iterator& Filter::Iterator::operator++()
{
    return Increment();
}

bool Filter::Iterator::operator!=(const Iterator &other) const
{
    return _current != other._current || &_world != &other._world;
}

Filter::Iterator Filter::begin()
{
    // ToDo: Ищем наименьшее по количеству сущностей хранилище компонентов

    // ToDo: создаем и возвращаем итератор по этому хранилищу, который указывает на
    // первую подходящую сущность
}

Filter::Iterator Filter::end()
{
    // ToDo: создаем и возвращаем итератор по минимальному хранилищу, который указывает на
    // конец итерирования (current = minStorageEntities.size())
}
