#include "Component.h"


void Component::SetOwner(Character* newOwner)
{
	owner = newOwner;
}

Character* Component::GetOwner()
{
	return owner;
}