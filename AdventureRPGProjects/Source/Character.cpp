#include "Character.h"
#include "StatusComponent.h"
#include "AbilityComponent.h"


std::string Character::GetName() const
{
	return name;
}

void Character::AddComponent(std::unique_ptr<Component> component)
{
	component->SetOwner(this);
	components.push_back(std::move(component));
}

