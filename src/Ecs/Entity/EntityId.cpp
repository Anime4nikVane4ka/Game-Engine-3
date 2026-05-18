#include "EntityId.h"

EntityId::EntityId(const int id, const int gen): _gen(gen), Id(id)
{
}

void EntityId::AddComponent(const int component)
{
    // ToDo: добавление компонента в список компонентов
    _components.push_back(component);
}

int EntityId::RemoveComponent(const int component)
{
    for (int i = 0; i < _components.size(); i++)
    {
        if (_components[i] != component)
            continue;

        _components[i] = _components[_components.size() - 1];
        _components.pop_back();
        return _components.size();
    }

    return _components.size();
}

bool EntityId::IsRemoved() const
{
    // ToDo: проверка жива сущность или нет (через поколение)
    return _gen < 0;
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
    _gen = -(_gen + 1);
    _components.clear();
}

void EntityId::Recycle()
{
    // ToDo: "переработка" сущности через восстановление валидного поколения
    _gen *= -1;
}

bool EntityId::Equals(const EntityId &other) const
{
    // ToDo: сравнение id и поколения сущностей
    return Id == other.Id && _gen == other._gen;
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
    os << "EntityId(Id: " << eId.Id << ", Gen: " << eId.Gen() << ")";
    return os;
}

