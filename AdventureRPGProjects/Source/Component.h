#pragma once
#include "Utils.h"

class Character;

class Component
{
protected:
	Character* owner;
public:
	Component(Character* character) : owner(character) {}
	virtual ~Component() {}

	void SetOwner(Character* newOwner);
	Character* GetOwner();
};
