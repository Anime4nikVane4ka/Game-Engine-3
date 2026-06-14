#ifndef CONFIG_H
#define CONFIG_H

#include <string>
#include <vector>

struct PlayerConfig {
    float BboxWidth = 0.0f;
    float BboxHeight = 0.0f;
    float MoveSpeed = 0.0f;
    float JumpForce = 0.0f;
    float MaxVelocity = 0.0f;
    float Gravity = 0.0f;
    std::string BasePose;
    std::vector<std::string> Animations;
};

struct GoombaConfig {
    float BboxWidth = 0.0f;
    float BboxHeight = 0.0f;
    float MoveSpeed = 0.0f;
    float JumpForce = 0.0f;
    float MaxVelocity = 0.0f;
    float Gravity = 0.0f;
    float ViewDistance = 0.0f;
    std::string BasePose;
    std::vector<std::string> Animations;
};

struct BulletConfig {
    std::string BaseTexture;
    float Speed = 0.0f;
    float Radius = 0.0f;
};

struct DecorationConfig {
    std::string Name;
    std::string BaseTexture;
    std::vector<std::string> Animations;
};

struct TileConfig {
    std::string BaseTexture;
};

struct BrickTileConfig {
    std::string BaseTexture;
    std::string DestroyAnimation;
};

struct QuestionTileConfig {
    std::string BaseTexture;
    std::string InactiveTexture;
    std::string CoinAnimation;
};

class Config {
  public:
    PlayerConfig Player;
    GoombaConfig Goomba;
    BulletConfig Bullet;
    std::vector<DecorationConfig> Decorations;
    TileConfig Tile;
    BrickTileConfig BrickTile;
    QuestionTileConfig QuestionTile;

    Config() = default;
    Config(const std::string& path);

    void LoadFromFile(const std::string& path);
};

#endif // CONFIG_H
