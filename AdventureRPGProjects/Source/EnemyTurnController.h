#pragma once
#include "CombatContext.h"

class CombatView;

class EnemyTurnController
{
public:
    void StartTurn(CombatContext& context, CombatView& view);
};
