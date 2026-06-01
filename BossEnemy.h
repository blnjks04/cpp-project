#pragma once
#include "Enemy.h"
#include "PokerCard.h"

enum class BossPhase2Skill
{
    NONE,
    HAND_TRICK,
    JOKER_GIFT,
    CORRUPTED_CARD
};

class BossEnemy : public Enemy
{
private:
    bool bIsPhase2 = false;
    int phase2TurnCount = 0;
    BossPhase2Skill nextPhase2Skill = BossPhase2Skill::NONE;

    int stolenRerollTurns = 0;
    int stolenRerollAmount = 0;
    bool bGiftPending = false;
    bool bGiftActiveThisTurn = false;
    int giftBonusRerolls = 2;
    int giftPenaltyDamage = 22;
    int giftPenaltyAttack = 1;
    PokerHandRank giftRequiredRank = PokerHandRank::THREE_OF_A_KIND;
    int corruptedCardTurns = 0;
    bool bCorruptedCardActiveThisTurn = false;
    int corruptedCardDamage = 4;

    void EnterPhase2();
    BossPhase2Skill PickRandomPhase2Skill() const;
    std::string GetPhase2SkillName(BossPhase2Skill skill) const;

public:
    BossEnemy(const std::string& name, int level = 1) : Enemy(name, level, EnemyType::BOSS) {};
    virtual ~BossEnemy() = default;

    bool IsPhase2() const;
    virtual void TakeDamage(int damage) override;
    virtual void ExecuteFSM(Player* player) override;
    virtual std::string GetNextAction() override;
    virtual void SetNextAction() override;

    std::string ApplyStartPlayerTurnEffect(Player* player);
    std::string OnPlayerReroll(Player* player);
    std::string ResolveGiftPenalty(PokerHandRank playerRank, Player* player);
};
