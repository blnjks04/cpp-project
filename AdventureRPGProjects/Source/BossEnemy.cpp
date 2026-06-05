#include "BossEnemy.h"
#include "EventDispatcher.h"
#include "Logger.h"
#include "Player.h"
#include "GameDataManager.h"

bool BossEnemy::IsPhase2() const
{
    return bIsPhase2;
}

void BossEnemy::TakeDamage(int damage)
{
    Enemy::TakeDamage(damage);

    if (!bIsPhase2 && Status->IsAlive() && Status->GetCurrentHp() <= Status->GetMaxHp() / 2)
    {
        EnterPhase2();
    }
}

void BossEnemy::ExecuteFSM(Player* player)
{
    if (!bIsPhase2)
    {
        DoAttack(player);
        return;
    }

    if (nextPhase2Skill == BossPhase2Skill::NONE)
    {
        DoAttack(player);
        phase2TurnCount++;
        return;
    }

    switch (nextPhase2Skill)
    {
    case BossPhase2Skill::HAND_TRICK:
        stolenRerollTurns = 2;
        stolenRerollAmount = 1;
        SetLastActionMessage(std::format("{}가 [조커의 손장난]을 사용했습니다. 2턴 동안 리롤 1회를 빼앗습니다.", GetName()));
        ADD_LOG(LogType::BattleEvent, GetLastActionMessage());
        break;
    case BossPhase2Skill::JOKER_GIFT:
        bGiftPending = true;
        SetLastActionMessage(std::format("{}가 [광대의 선물]을 건넸습니다. 다음 턴 TWO PAIR 이상에 실패하면 큰 페널티를 받습니다.", GetName()));
        ADD_LOG(LogType::BattleEvent, GetLastActionMessage());
        break;
    case BossPhase2Skill::CORRUPTED_CARD:
        corruptedCardTurns = 3;
        SetLastActionMessage(std::format("{}가 [오염된 카드]를 흩뿌렸습니다. 3턴 동안 리롤할 때마다 피해를 받습니다.", GetName()));
        ADD_LOG(LogType::BattleEvent, GetLastActionMessage());
        break;
    default:
        DoAttack(player);
        break;
    }

    nextPhase2Skill = BossPhase2Skill::NONE;
    phase2TurnCount++;
}

std::string BossEnemy::GetNextAction()
{
    if (!bIsPhase2)
    {
        return std::format("공격 {}", GetBaseDamage());
    }
    if (nextPhase2Skill == BossPhase2Skill::NONE)
    {
        return std::format("공격 {}", GetBaseDamage());
    }
    return GetPhase2SkillName(nextPhase2Skill);
}

void BossEnemy::SetNextAction()
{
    if (!bIsPhase2)
    {
        nextPhase2Skill = BossPhase2Skill::NONE;
        return;
    }

    if (phase2TurnCount % 2 == 0)
    {
        nextPhase2Skill = BossPhase2Skill::NONE;
        return;
    }
    nextPhase2Skill = PickRandomPhase2Skill();
}

std::string BossEnemy::ApplyStartPlayerTurnEffect(Player* player)
{
    if (!bIsPhase2 || player == nullptr) return "";

    std::string message;
    bGiftActiveThisTurn = false;
    bCorruptedCardActiveThisTurn = false;

    if (stolenRerollTurns > 0)
    {
        player->SetRerolls(player->GetRerolls() - stolenRerollAmount);
        stolenRerollTurns--;
        message.append(std::format("[조커의 손장난] 리롤 {}회를 빼앗겼습니다. ", stolenRerollAmount));
    }

    if (bGiftPending)
    {
        player->SetRerolls(player->GetRerolls() + giftBonusRerolls);
        bGiftPending = false;
        bGiftActiveThisTurn = true;
        message.append(std::format("[광대의 선물] 리롤 +{} 획득. TWO PAIR 이상을 만들어야 합니다. ", giftBonusRerolls));
    }

    if (corruptedCardTurns > 0)
    {
        bCorruptedCardActiveThisTurn = true;
        corruptedCardTurns--;
        message.append(std::format("[오염된 카드] 이번 턴 리롤마다 {} 피해를 받습니다. ", corruptedCardDamage));
    }

    return message;
}

std::string BossEnemy::OnPlayerReroll(Player* player)
{
    if (!bIsPhase2 || player == nullptr || !bCorruptedCardActiveThisTurn) return "";

    player->TakeDamage(corruptedCardDamage);
    ADD_LOG(LogType::BattleEvent, std::format("[오염된 카드] 리롤 피해 {} 적용", corruptedCardDamage));
    return std::format("[오염된 카드] 리롤의 대가로 {} 피해를 입었습니다.", corruptedCardDamage);
}

std::string BossEnemy::ResolveGiftPenalty(PokerHandRank playerRank, Player* player)
{
    if (!bIsPhase2 || player == nullptr || !bGiftActiveThisTurn) return "";

    bGiftActiveThisTurn = false;
    if (static_cast<int>(playerRank) >= static_cast<int>(giftRequiredRank))
    {
        return "[광대의 선물] 조건 달성! 페널티를 피했습니다.";
    }

    player->TakeDamage(giftPenaltyDamage);
    player->AddBaseDamage(-giftPenaltyAttack);
    ADD_LOG(LogType::BattleEvent, std::format("[광대의 선물] 실패 페널티: 피해 {}, 공격력 {} 감소", giftPenaltyDamage, giftPenaltyAttack));
    return std::format("[광대의 선물] 실패! {} 피해와 공격력 {} 감소를 받았습니다.", giftPenaltyDamage, giftPenaltyAttack);
}

void BossEnemy::EnterPhase2()
{
    bIsPhase2 = true;
    Status->SetBlock(Status->GetBlock() + 35);
    
    EnemyData* phase2Boss = GameDataManager::GetInstance()->GetEnemyData(EnemyRank::BOSS, "JokerCard");
    if (phase2Boss != nullptr)
    {
        Status->SetMaxHp(phase2Boss->hp);
        Status->SetCurrentHp(Status->GetMaxHp());
        SetBaseDamage(phase2Boss->attack);
        SetSprite(phase2Boss->sprite);
        SetAnimSprite(phase2Boss->animSprite);
    }
    
    nextPhase2Skill = BossPhase2Skill::NONE;

    SetLastActionMessage(std::format("{}가 [색을 되찾은 광대]로 변신했습니다. 공격력과 방어막이 증가합니다.", GetName()));
    ADD_LOG(LogType::BattleEvent, GetLastActionMessage());
}

BossPhase2Skill BossEnemy::PickRandomPhase2Skill() const
{
    std::random_device randomDevice;
    std::mt19937 randomGenerator(randomDevice());
    std::uniform_int_distribution<int> skillIndexDistribution(0, 2);

    switch (skillIndexDistribution(randomGenerator))
    {
    case 0:
        return BossPhase2Skill::HAND_TRICK;
    case 1:
        return BossPhase2Skill::JOKER_GIFT;
    default:
        return BossPhase2Skill::CORRUPTED_CARD;
    }
}

std::string BossEnemy::GetPhase2SkillName(BossPhase2Skill skill) const
{
    switch (skill)
    {
    case BossPhase2Skill::HAND_TRICK:
        return "조커의 손장난";
    case BossPhase2Skill::JOKER_GIFT:
        return "광대의 선물";
    case BossPhase2Skill::CORRUPTED_CARD:
        return "오염된 카드";
    default:
        return "공격";
    }
}
