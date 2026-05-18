#ifndef CONFIG_H
#define CONFIG_H

#include <string>
#include <map>
#include <fstream>
#include <iostream>

class Config
{
private:
    std::map<std::string, std::string> _data;

public:
    Config() = default;
    Config(const std::string& path);

    void LoadFromFile(const std::string& path);

    std::string getString(const std::string& key) const;
    int getInt(const std::string& key) const;
    float getFloat(const std::string& key) const;
    bool getBool(const std::string& key) const;
    float getFloatArrayValue(const std::string& key, int index) const;
};

#endif // CONFIG_H