#pragma once
#include "Utils.h"

class Player;
class Enemy;

class CombatManager
{
public:
	CombatManager() = default;
	~CombatManager() = default;

	void StartBattle(Player* player, std::vector<Enemy*>& enemies);
};
