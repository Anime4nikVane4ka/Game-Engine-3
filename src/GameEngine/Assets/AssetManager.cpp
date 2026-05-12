#include "AssetManager.h"

#include <fstream>
#include <iostream>

void AssetManager::LoadFromFile(const std::string &path)
{
    // ToDo: Логика загрузки ассетов из файла конфигурации
}

void AssetManager::AddTexture(const std::string &name, const std::string &path)
{
    // ToDo: Логика загрузки текстуры из файла
}

void AssetManager::AddAnimation(const std::string &name, const std::string &textureName, const size_t frameCount,
    const size_t frameDuration)
{
    // ToDo: Логика загрузки анимации из файла
}

void AssetManager::AddFont(const std::string &name, const std::string &path)
{
    // ToDo: Логика загрузки шрифта из файла
}

const sf::Texture& AssetManager::GetTexture(const std::string &name) const
{
    // ToDo: Логика получения текстуры из мапы
}

const Animation& AssetManager::GetAnimation(const std::string &name) const
{
    // ToDo: Логика получения анимации из мапы
}

const sf::Font& AssetManager::GetFont(const std::string &name) const
{
    // ToDo: Логика получения шрифта из мапы
}



