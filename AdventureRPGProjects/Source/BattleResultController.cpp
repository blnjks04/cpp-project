#include "BattleResultController.h"
#include "BattleResultView.h"
#include "BattleRewardController.h"
#include "ConsoleFramework.h"
#include "GameStateManager.h"
#include "Player.h"

void BattleResultController::Resolve(CombatContext& context)
{
    if (context.bIsVictory)
    {
        ResolveVictory(context);
    }
    else
    {
        ResolveDefeatOrEscape(context);
    }
}

void BattleResultController::ResolveVictory(CombatContext& context)
{
    context.player->LevelUp();
    if (context.battleState == GameState::ELITE_BATTLE)
    {
        context.player->LevelUp();
    }

    CLEAR_SCREEN();
    BattleResultView resultView;
    if (context.battleState == GameState::BOSS_BATTLE)
    {
        resultView.DisplayBossClear(context.player, context.enemies);
        GET_MOUSE_CLICK();
        CHANGE_STATE(GameState::GAME_EXIT);
        return;
    }

    BattleRewardController rewardController;
    std::vector<BattleRewardOption> rewards = rewardController.BuildRewards(context.battleState);
    resultView.DisplayBattleResult(context.player, context.battleResult, context.enemies, context.battleState, rewards);

    while (true)
    {
        MouseClick rewardClick = GET_MOUSE_CLICK();
        int rewardIndex = rewardController.GetClickedRewardIndex(rewardClick, rewards);
        if (rewardIndex < 0) continue;

        rewardController.ApplyReward(context.player, rewards[rewardIndex]);
        resultView.DisplayRewardApplied(context.player, rewards[rewardIndex]);
        GET_MOUSE_CLICK();
        break;
    }

    CHANGE_STATE(GameState::MAP);
}

void BattleResultController::ResolveDefeatOrEscape(CombatContext& context)
{
    int nextHp = context.player->Status->GetCurrentHp() - (context.player->Status->GetMaxHp() / 2);
    if (context.battleResult == "탈출" && nextHp < 1) nextHp = 1;
    if (context.battleResult != "탈출" && nextHp < 0) nextHp = 0;
    context.player->Status->SetCurrentHp(nextHp);
    if (nextHp == 0) context.player->Status->SetBIsAlive(false);

    CLEAR_SCREEN();
    BattleResultView resultView;
    std::vector<BattleRewardOption> rewards;
    resultView.DisplayBattleResult(context.player, context.battleResult, context.enemies, context.battleState, rewards);
    GET_MOUSE_CLICK();

    CHANGE_STATE(GameState::MAP);
}
