#include "AssetManager.h"

#include <fstream>
#include <stdexcept>
#include <utility>

#include <nlohmann/json.hpp>

void AssetManager::LoadFromFile(const std::string &path)
{
    // ToDo: Логика загрузки ассетов из файла конфигурации
    std::ifstream file(path);
    if (!file.is_open())
        throw std::runtime_error("Can not open assets file: " + path);

    nlohmann::json assets;
    file >> assets;

    for (const auto& texture : assets.at("Textures"))
    {
        AddTexture(
            texture.at("name").get<std::string>(),
            texture.at("path").get<std::string>());
    }

    for (const auto& animation : assets.at("Animations"))
    {
        AddAnimation(
            animation.at("name").get<std::string>(),
            animation.at("textureName").get<std::string>(),
            animation.at("frameCount").get<size_t>(),
            animation.at("frameDuration").get<size_t>());
    }

    for (const auto& font : assets.at("Fonts"))
    {
        AddFont(
            font.at("name").get<std::string>(),
            font.at("path").get<std::string>());
    }
}

void AssetManager::AddTexture(const std::string &name, const std::string &path)
{
    // ToDo: Логика загрузки текстуры из файла
    sf::Texture texture;
    if (!texture.loadFromFile(path))
        throw std::runtime_error("Can not load texture: " + path);

    _textures.erase(name);
    _textures.emplace(name, std::move(texture));
}

void AssetManager::AddAnimation(const std::string &name, const std::string &textureName, const size_t frameCount,
    const size_t frameDuration)
{
    // ToDo: Логика загрузки анимации из файла
    _animations.erase(name);
    _animations.emplace(name, Animation(GetTexture(textureName), frameCount, frameDuration));
}

void AssetManager::AddFont(const std::string &name, const std::string &path)
{
    // ToDo: Логика загрузки шрифта из файла
    sf::Font font;
    if (!font.openFromFile(path))
        throw std::runtime_error("Can not load font: " + path);

    _fonts.erase(name);
    _fonts.emplace(name, std::move(font));
}

const sf::Texture& AssetManager::GetTexture(const std::string &name) const
{
    // ToDo: Логика получения текстуры из мапы
    const auto textureIterator = _textures.find(name);
    if (textureIterator == _textures.end())
        throw std::runtime_error("Texture not found: " + name);

    return textureIterator->second;
}

const Animation& AssetManager::GetAnimation(const std::string &name) const
{
    // ToDo: Логика получения анимации из мапы
    const auto animationIterator = _animations.find(name);
    if (animationIterator == _animations.end())
        throw std::runtime_error("Animation not found: " + name);

    return animationIterator->second;
}

const sf::Font& AssetManager::GetFont(const std::string &name) const
{
    // ToDo: Логика получения шрифта из мапы
    const auto fontIterator = _fonts.find(name);
    if (fontIterator == _fonts.end())
        throw std::runtime_error("Font not found: " + name);

    return fontIterator->second;
}

