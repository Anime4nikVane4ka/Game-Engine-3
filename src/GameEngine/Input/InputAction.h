#ifndef INPUTACTION_H
#define INPUTACTION_H
#include <string>

#include "SFML/System/Vector2.hpp"

enum ActionType { None, Start, End };

class InputAction {
    std::string _name;
    ActionType _type = None;

    short _value = 0;
    sf::Vector2i _value2 = {0, 0};

  public:
    InputAction() = default;
    InputAction(const std::string& name) : _name(name) {}

    const std::string& Name() const {
        return _name;
    }
    ActionType& Type() {
        return _type;
    }
    short& Value() {
        return _value;
    }
    sf::Vector2i& Value2() {
        return _value2;
    }
};

#endif // INPUTACTION_H
