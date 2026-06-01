#pragma once
#include "Button.h"

class BattleResultButtons
{
public:
    static Button RewardOption(int index, const std::string& title = "", const std::string& description = "");
    static Button BossExit();
};
