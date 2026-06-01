#include "BossPhase2RuleEffect.h"
#include "BossEnemy.h"
#include "Player.h"

BossPhase2RuleEffect::BossPhase2RuleEffect(BossEnemy* bossEnemy)
    : boss(bossEnemy)
{
}

void BossPhase2RuleEffect::OnPlayerTurnStart(CombatContext& context)
{
    if (boss == nullptr || context.player == nullptr) return;

    lastMessage = boss->ApplyStartPlayerTurnEffect(context.player);
}

void BossPhase2RuleEffect::OnPlayerReroll(CombatContext& context)
{
    if (boss == nullptr || context.player == nullptr) return;

    lastMessage = boss->OnPlayerReroll(context.player);
}

void BossPhase2RuleEffect::OnHandConfirmed(CombatContext& context)
{
    if (boss == nullptr || context.player == nullptr) return;

    lastMessage = boss->ResolveGiftPenalty(context.handResult.rank, context.player);
}

std::string BossPhase2RuleEffect::GetLastMessage() const
{
    return lastMessage;
}
