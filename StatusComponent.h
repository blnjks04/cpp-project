#pragma once
#include "Utils.h"
#include "Component.h"	



class StatusComponent : public Component
{
private:
	int Hp = 60;
	int MaxHp = 60;
	int Block = 0;
	bool bAlive = true;
public:
    StatusComponent(Character* character) : Component(character) {};
    ~StatusComponent() = default;
	int GetCurrentHp() const;
	void SetCurrentHp(int newHp);
	int GetMaxHp() const;
	void SetMaxHp(int newMaxHp);
	int GetBlock() const;
	void SetBlock(int newBlock);
	bool IsAlive() const;
	void SetBIsAlive(bool newBIsAlive);
};
