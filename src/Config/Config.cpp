#include "Config.h"

#include <fstream>
#include <stdexcept>

#include <nlohmann/json.hpp>

std::vector<std::string> ReadStringArray(const nlohmann::json& json) {
    std::vector<std::string> result;

    for (const auto& value : json)
        result.push_back(value.get<std::string>());

    return result;
}

std::ifstream OpenFile(const std::string& path) {
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

Config::Config(const std::string& path) {
    LoadFromFile(path);
}

void Config::LoadFromFile(const std::string& path) {
    std::ifstream file = OpenFile(path);
    if (!file.is_open())
        throw std::runtime_error("Could not open config file: " + path);

    nlohmann::json json;
    file >> json;

    const auto& player = json.at("Player");
    const auto& bbox = player.at("Bbox");
    Player.BboxWidth = bbox.at(0).get<float>();
    Player.BboxHeight = bbox.at(1).get<float>();
    Player.MoveSpeed = player.at("MoveSpeed").get<float>();
    Player.JumpForce = player.at("JumpForce").get<float>();
    Player.MaxVelocity = player.at("MaxVelocity").get<float>();
    Player.Gravity = player.at("Gravity").get<float>();
    Player.BasePose = player.at("BasePose").get<std::string>();
    Player.Animations = ReadStringArray(player.at("Animations"));

    const auto& goomba = json.at("Goomba");
    const auto& goombaBbox = goomba.at("Bbox");
    Goomba.BboxWidth = goombaBbox.at(0).get<float>();
    Goomba.BboxHeight = goombaBbox.at(1).get<float>();
    Goomba.MoveSpeed = goomba.at("MoveSpeed").get<float>();
    Goomba.JumpForce = goomba.at("JumpForce").get<float>();
    Goomba.MaxVelocity = goomba.at("MaxVelocity").get<float>();
    Goomba.Gravity = goomba.at("Gravity").get<float>();
    Goomba.ViewDistance = goomba.at("ViewDistance").get<float>();
    Goomba.BasePose = goomba.at("BasePose").get<std::string>();
    Goomba.Animations = ReadStringArray(goomba.at("Animations"));

    const auto& bullet = json.at("Bullet");
    Bullet.BaseTexture = bullet.at("BaseTexture").get<std::string>();
    Bullet.Speed = bullet.at("Speed").get<float>();
    Bullet.Radius = bullet.at("Radius").get<float>();

    Decorations.clear();
    for (const auto& decoration : json.at("Decorations")) {
        DecorationConfig decorationConfig;
        decorationConfig.Name = decoration.at("Name").get<std::string>();
        decorationConfig.BaseTexture = decoration.at("BaseTexture").get<std::string>();

        if (decoration.contains("Animations"))
            decorationConfig.Animations = ReadStringArray(decoration.at("Animations"));

        Decorations.push_back(decorationConfig);
    }

    const auto& tile = json.at("Tile");
    Tile.BaseTexture = tile.at("BaseTexture").get<std::string>();

    const auto& brickTile = json.at("BrickTile");
    BrickTile.BaseTexture = brickTile.at("BaseTexture").get<std::string>();
    BrickTile.DestroyAnimation = brickTile.at("DestroyAnimation").get<std::string>();

    const auto& questionTile = json.at("QuestionTile");
    QuestionTile.BaseTexture = questionTile.at("BaseTexture").get<std::string>();
    QuestionTile.InactiveTexture = questionTile.at("InactiveTexture").get<std::string>();
    QuestionTile.CoinAnimation = questionTile.at("CoinAnimation").get<std::string>();
}
