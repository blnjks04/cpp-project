#pragma once
#include "BattleRewardOption.h"
#include "ConsoleFramework.h"
#include "GameStateManager.h"

class Player;

class BattleRewardController
{

public:
    BattleRewardController() = default;
    ~BattleRewardController() = default;

public:

    std::vector<BattleRewardOption> BuildRewards(GameState battleState) const;
    int GetClickedRewardIndex(MouseClick click, const std::vector<BattleRewardOption>& rewards) const;
    void ApplyReward(Player* player, const BattleRewardOption& reward) const;
};
