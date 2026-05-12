#include "EntityId.h"

void EntityId::AddComponent(const int component)
{
    // ToDo: добавление компонента в список компонентов
}

int EntityId::RemoveComponent(const int component)
{
    // ToDo: FastRemove компонента из указанного списка
}

bool EntityId::IsRemoved() const
{
    // ToDo: проверка жива сущность или нет (через поколение)
}

const std::vector<int>& EntityId::Components() const
{
    return _components;
}

int EntityId::Gen() const
{
    return _gen;
}

void EntityId::Remove()
{
    // ToDo: удаление сущности (через инвалидацию поколения) и очистка списка компонентов
}

void EntityId::Recycle()
{
    // ToDo: "переработка" сущности через восстановление валидного поколения
}

bool EntityId::Equals(const EntityId &other) const
{
    // ToDo: сравнение id и поколения сущностей
}

bool EntityId::operator==(const EntityId &other) const
{
    return this->Equals(other);
}

bool EntityId::operator!=(const EntityId &other) const
{
    return !this->Equals(other);
}

std::ostream& operator<<(std::ostream &os, const EntityId &eId)
{
    // ToDo: Красивый ввывод сущности в строчку
}
