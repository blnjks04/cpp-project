#include "StatusComponent.h"


int StatusComponent::GetCurrentHp() const
{
	return Hp;
}

void StatusComponent::SetCurrentHp(int newHp)
{
	Hp = newHp;
}

int StatusComponent::GetMaxHp() const
{
	return MaxHp;
}

void StatusComponent::SetMaxHp(int newMaxHp)
{
	MaxHp = newMaxHp;
}

int StatusComponent::GetBlock() const
{
	return Block;
}

void StatusComponent::SetBlock(int newBlock)
{
	Block = newBlock;
}

bool StatusComponent::IsAlive() const
{
	return bAlive;
}

void StatusComponent::SetBIsAlive(bool newBIsAlive)
{
	bAlive = newBIsAlive;
}