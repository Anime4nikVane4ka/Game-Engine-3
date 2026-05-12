#include "SystemsManager.h"

SystemsManager& SystemsManager::AddInitializer(std::shared_ptr<IInitializer> initializer)
{
    // ToDo: Логика добавления системы-инициализатора в менеджер
}

SystemsManager& SystemsManager::AddSystem(std::shared_ptr<ISystem> system)
{
    // ToDo: Логика добавления системы в менеджер
}

void SystemsManager::Initialize()
{
    // ToDo: Логика вызова метода Initialize у еще не вызывавшихся инициализаторов
}

void SystemsManager::Update()
{
    // ToDo: Логика вызова метода Update у систем
}
