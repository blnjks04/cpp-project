#pragma once
#include "Utils.h"
#include "Component.h"


class AbilityComponent : public Component
{
public:
    AbilityComponent(Character* character) : Component(character) {};
    ~AbilityComponent() = default;
};