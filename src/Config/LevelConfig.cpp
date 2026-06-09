#include "LevelConfig.h"

#include <fstream>
#include <stdexcept>

#include <nlohmann/json.hpp>

std::ifstream OpenLevelFile(const std::string& path) {
    std::ifstream file(path);
    if (file.is_open())
        return file;

    file.clear();
    file.open("../" + path);
    if (file.is_open())
        return file;

    file.clear();
    file.open("../../" + path);
    return file;
}

LevelConfig::LevelConfig(const std::string& path) {
    LoadFromFile(path);
}

void LevelConfig::LoadFromFile(const std::string& path) {
    std::ifstream file = OpenLevelFile(path);
    if (!file.is_open())
        throw std::runtime_error("Could not open level config file: " + path);

    nlohmann::json json;
    file >> json;

    Width = json.at("width").get<int>();
    Height = json.at("height").get<int>();

    Objects.clear();
    for (const auto& object : json.at("objects")) {
        LevelObjectConfig levelObject;
        levelObject.Name = object.at("name").get<std::string>();
        levelObject.GridX = object.at("x").get<int>();
        levelObject.GridY = object.at("y").get<int>();
        levelObject.X = levelObject.GridX * CellSize;
        levelObject.Y = (Height - levelObject.GridY) * CellSize;

        Objects.push_back(levelObject);
    }
}
