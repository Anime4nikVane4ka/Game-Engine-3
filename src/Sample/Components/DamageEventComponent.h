#ifndef DAMAGEEVENTCOMPONENT_H
#define DAMAGEEVENTCOMPONENT_H

struct DamageEventComponent {
    int FirstEntity = -1;
    int SecondEntity = -1;

    DamageEventComponent() = default;
    DamageEventComponent(const int firstEntity, const int secondEntity) : FirstEntity(firstEntity), SecondEntity(secondEntity) {}
};

#endif // DAMAGEEVENTCOMPONENT_H
