#pragma once
#include "BattleRewardOption.h"
#include "GameStateManager.h"
#include "View.h"

class Enemy;
class Player;

class BattleResultView : public View
{
public:
    void DisplayBattleResult(Player* player, const std::string& battleResult, const std::vector<Enemy*>& enemies, GameState battleState, const std::vector<BattleRewardOption>& rewards);
    void DisplayRewardApplied(Player* player, const BattleRewardOption& reward);
    void DisplayBossClear(Player* player, const std::vector<Enemy*>& enemies);
};
