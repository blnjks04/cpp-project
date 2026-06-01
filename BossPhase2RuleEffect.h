#pragma once
#include "CombatRuleEffect.h"

class BossEnemy;

class BossPhase2RuleEffect : public CombatRuleEffect
{
private:
    BossEnemy* boss = nullptr;
    std::string lastMessage;

public:
    explicit BossPhase2RuleEffect(BossEnemy* bossEnemy);

    virtual void OnPlayerTurnStart(CombatContext& context) override;
    virtual void OnPlayerReroll(CombatContext& context) override;
    virtual void OnHandConfirmed(CombatContext& context) override;
    virtual std::string GetLastMessage() const override;
};
