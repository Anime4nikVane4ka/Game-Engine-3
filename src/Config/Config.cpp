#include "Config.h"

#include <algorithm>
#include <charconv>

Config::Config(const std::string& path)
{
    LoadFromFile(path);
}

void Config::LoadFromFile(const std::string& path)
{
    _data.clear();

    std::ifstream file(path);
    if (!file.is_open())
    {
        file.clear();
        file.open("../" + path);
    }
    if (!file.is_open())
    {
        file.clear();
        file.open("../../" + path);
    }
    if (!file.is_open())
    {
        std::cout << "Could not open config file: " << path << "\n";
        return;
    }

    std::string line;
    while (std::getline(file, line))
    {
        if (line.empty())
            continue;
        std::size_t eqPos = line.find('=');
        if (eqPos == std::string::npos)
            continue;

        std::string key = line.substr(0, eqPos);
        std::string value = line.substr(eqPos + 1);
        //std::cout << key << "..." << value << std::endl;

        _data[key] = value;
    }
}

std::string Config::getString(const std::string& key) const
{
    return _data.at(key);    
}

int Config::getInt(const std::string& key) const
{
    return std::stoi(_data.at(key));
}

float Config::getFloat(const std::string& key) const
{
    std::string value = _data.at(key);
    std::replace(value.begin(), value.end(), ',', '.');

    float result = 0.0f;
    std::from_chars(value.data(), value.data() + value.size(), result);
    return result;
}

bool Config::getBool(const std::string& key) const
{
    std::string flag = _data.at(key);

    if (flag == "true" || flag == "1")
        return true;

    if (flag == "false" || flag == "0")
        return false;

    return false;
}

#include <sstream>
#include <string>

float Config::getFloatArrayValue(const std::string& key, int index) const
{
    std::string line = _data.at(key);
    std::size_t start = 0;
    int currentIndex = 0;
    while (start <= line.size())
    {
        std::size_t commaPos = line.find(',', start);
        std::string part;
        if (commaPos == std::string::npos)
            part = line.substr(start);
        else
            part = line.substr(start, commaPos - start);
        if (currentIndex == index)
        {
            std::replace(part.begin(), part.end(), ',', '.');

            float result = 0.0f;
            std::from_chars(part.data(), part.data() + part.size(), result);
            return result;
        }
        start = commaPos + 1;
        ++currentIndex;
    }

    return 0.0f;
}
