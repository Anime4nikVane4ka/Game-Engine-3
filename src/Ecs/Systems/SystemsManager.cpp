#include "SystemsManager.h"

SystemsManager::SystemsManager(World& world): _world(world)
{
}

SystemsManager& SystemsManager::AddInitializer(std::shared_ptr<IInitializer> initializer)
{
    // ToDo: Логика добавления системы-инициализатора в менеджер
    _newInitializers.push_back(initializer);
    return *this;
}

SystemsManager& SystemsManager::AddSystem(std::shared_ptr<ISystem> system)
{
    // ToDo: Логика добавления системы в менеджер
    _newInitializers.push_back(system);
    _systems.push_back(system);
    return *this;
}

void SystemsManager::Initialize()
{
    // ToDo: Логика вызова метода Initialize у еще не вызывавшихся инициализаторов
    if (_newInitializers.empty())
        return;
    for (int i = 0; i < _newInitializers.size(); i++)
    {
        _newInitializers[i]->OnInit();
        _initializers.push_back(_newInitializers[i]);
    }
    _newInitializers.clear();
}

void SystemsManager::Update()
{
    // ToDo: Логика вызова метода Update у систем
    Initialize();
    for (int i = 0; i < _systems.size(); i++)
    {
        _systems[i]->OnUpdate();
    }
}