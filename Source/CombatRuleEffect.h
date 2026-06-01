#pragma once
#include "Utils.h"
#include "CombatContext.h"

class CombatRuleEffect
{
public:
    
    virtual ~CombatRuleEffect() = default;

    virtual void OnPlayerTurnStart(CombatContext& context) {};
    virtual void OnPlayerReroll(CombatContext& context) {};
    virtual void OnHandConfirmed(CombatContext& context) {};
    virtual void OnEnemyTurnStart(CombatContext& context) {};
    virtual std::string GetLastMessage() const;
};
