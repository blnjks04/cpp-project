#pragma once
#include "CombatContext.h"

class BattleResultController
{
public:
    BattleResultController() = default;
    ~BattleResultController() = default;

    void Resolve(CombatContext& context);

private:
    void ResolveVictory(CombatContext& context);
    void ResolveDefeatOrEscape(CombatContext& context);
};
