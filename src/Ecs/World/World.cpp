#include "World.h"

void World::EntityComponentsChanged(const int e, const int storageId, const bool added)
{
    // ToDo: Логика обновления маски на сущности при изменении набора компонент на ней
}

World::World()
{
    // ToDo: Инициализация мира и выделения памяти
}

int World::CreateEntity()
{
    // ToDo: Логика создания новой сущности или переиспользования свободной сущности
}

void World::RemoveEntity(int e)
{
    // ToDo: Логика удаления сущности из мира и вычистки ее из хранилищ компонентов
}

EntityId World::GetPackedEntity(const int e) const
{
    // ToDo: Логика получения упаковонной сущности для хранения в мире
}

bool World::UnpackEntity(const EntityId &eId, int &e) const
{
    // ToDo: Логика распаковки и валидации сущности
}

bool World::IsEntityAlive(const int e) const
{
    // ToDo: Логика проверки жива сущность и корректна или нет
}
