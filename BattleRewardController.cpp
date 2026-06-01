#include "BattleRewardController.h"
#include "Player.h"
#include "BattleResultButtons.h"
#include "ButtonRenderer.h"

std::vector<BattleRewardOption> BattleRewardController::BuildRewards(GameState battleState) const
{
    if (battleState == GameState::ELITE_BATTLE)
    {
        return {
            { "성벽 파쇄", "공격력 +2", 2, 0, 0, 0 },
            { "방어 자세", "최대 체력 +6, 현재 체력 +6", 0, 6, 6, 0 },
            { "보물 상자", "골드 +45", 0, 0, 0, 45 }
        };
    }

    return {
        { "전술 연마", "공격력 +1", 1, 0, 0, 0 },
        { "심호흡", "최대 체력 +3, 현재 체력 +3", 0, 3, 3, 0 },
        { "보물 상자", "골드 +20", 0, 0, 0, 20 }
    };
}

int BattleRewardController::GetClickedRewardIndex(MouseClick click, const std::vector<BattleRewardOption>& rewards) const
{
    for (int i = 0; i < static_cast<int>(rewards.size()); i++)
    {
        if (ButtonRenderer::IsClicked(BattleResultButtons::RewardOption(i), click))
        {
            return i;
        }
    }
    return -1;
}

void BattleRewardController::ApplyReward(Player* player, const BattleRewardOption& reward) const
{
    if (player == nullptr) return;

    if (reward.maxHpChange != 0)
    {
        player->Status->SetMaxHp(player->Status->GetMaxHp() + reward.maxHpChange);
    }
    if (reward.hpChange != 0)
    {
        int nextHp = player->Status->GetCurrentHp() + reward.hpChange;
        if (nextHp > player->Status->GetMaxHp()) nextHp = player->Status->GetMaxHp();
        player->Status->SetCurrentHp(nextHp);
    }
    if (reward.attackChange != 0)
    {
        player->AddBaseDamage(reward.attackChange);
    }
    if (reward.goldGain != 0)
    {
        player->GainGold(reward.goldGain);
    }
}
