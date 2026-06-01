#pragma once
#include <string>

struct BattleRewardOption
{
    std::string title;
    std::string description;
    int attackChange = 0;
    int maxHpChange = 0;
    int hpChange = 0;
    int goldGain = 0;
};
