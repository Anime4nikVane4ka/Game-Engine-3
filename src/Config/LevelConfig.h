#ifndef LEVELCONFIG_H
#define LEVELCONFIG_H

#include <string>
#include <vector>

struct LevelObjectConfig
{
    std::string Name;
    int GridX = 0;
    int GridY = 0;
    float X = 0.0f;
    float Y = 0.0f;
};

class LevelConfig
{
public:
    static constexpr int CellSize = 64;

    int Width = 0;
    int Height = 0;
    std::vector<LevelObjectConfig> Objects;

    LevelConfig() = default;
    LevelConfig(const std::string& path);

    void LoadFromFile(const std::string& path);
};

#endif //LEVELCONFIG_H
