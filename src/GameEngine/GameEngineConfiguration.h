#ifndef GAMEENGINECONF_H
#define GAMEENGINECONF_H

class GameEngineConfiguration
{
public:
    static constexpr unsigned int Width = 1280;
    static constexpr unsigned int Height = 768;
    static constexpr const char* AssetsFile = "assets.json";
    static constexpr const char* ConfigFile = "config.json";
    static constexpr const char* LevelFile = "level.json";
};

#endif //GAMEENGINECONF_H
