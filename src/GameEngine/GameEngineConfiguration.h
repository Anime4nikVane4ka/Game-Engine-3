#ifndef GAMEENGINECONF_H
#define GAMEENGINECONF_H

// ToDo: Ваш аналог конфига, который читается из файла
class GameEngineConfiguration
{
public:
    static constexpr unsigned int Width = 1280;
    static constexpr unsigned int Height = 768;

    static constexpr std::string AssetsFile = "assets.json";
};

#endif //GAMEENGINECONF_H
