#ifndef FILTERBUILDER_H
#define FILTERBUILDER_H

#include "Filter.h"
#include "../World/World.h"

class FilterBuilder
{
    World& _world;
    std::vector<std::shared_ptr<BaseComponentStorage>> _componentStorages;

public:
    // ToDo:
    FilterBuilder(World& world);

    template <typename T>
    FilterBuilder& With();

    Filter Build() const;
};

#endif //FILTERBUILDER_H
