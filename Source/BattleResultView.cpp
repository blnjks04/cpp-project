#include "BattleResultView.h"
#include "BattleResultButtons.h"
#include "Enemy.h"
#include "Player.h"

void BattleResultView::DisplayBattleResult(Player* player, const std::string& battleResult, const std::vector<Enemy*>& enemies, GameState battleState, const std::vector<BattleRewardOption>& rewards)
{
    std::string defeatedEnemyNames;
    for (const auto& enemy : enemies)
    {
        if (enemy == nullptr) continue;
        if (!defeatedEnemyNames.empty()) defeatedEnemyNames.append(", ");
        defeatedEnemyNames.append(enemy->GetName());
    }
    if (defeatedEnemyNames.empty()) defeatedEnemyNames = "없음";

    std::string title = "BATTLE RESULT";
    std::string resultLine = "전투가 끝났습니다.";
    if (battleState == GameState::ELITE_BATTLE)
    {
        title = "ELITE CLEARED";
        resultLine = "강력한 엘리트가 무너졌습니다.";
    }

    DrawText(0, 0, "========================================================================================================================");
    DrawText(52, 1, title, "yellow");
    DrawText(0, 2, "========================================================================================================================");
    DrawText(25, 4, resultLine, battleResult == "승리" ? "cyan" : "yellow");

    DrawText(16, 6, "+--------------------------------------------------------------------------------+");
    DrawText(16, 7, std::format("| 처치한 적        {:<61}|", defeatedEnemyNames));
    DrawText(16, 8, std::format("| 전투 결과        {:<61}|", battleResult));
    DrawText(16, 9, std::format("| 현재 상태        Level {} / HP {}/{} / ATK {} / GOLD : {:<25}|", player->GetLevel(), player->Status->GetCurrentHp(), player->Status->GetMaxHp(), player->GetBaseDamage(), player->GetGold()));
    DrawText(16, 10, "+--------------------------------------------------------------------------------+");

    if (battleResult == "승리")
    {
        DrawText(18, 12, "보상 선택", "yellow");
        for (int i = 0; i < rewards.size(); i++)
        {
            DrawButton(BattleResultButtons::RewardOption(i, rewards[i].title, rewards[i].description));
        }
        DrawText(0, 27, "========================================================================================================================");
        DrawText(31, 28, "원하는 보상을 클릭하면 적용 후 맵으로 돌아갑니다.", "yellow");
    }
    else if (battleResult == "패배")
    {
        DrawText(18, 13, "패배의 대가로 체력을 잃었습니다.", "red");
        DrawText(18, 15, "계속하려면 아무 곳이나 클릭하세요.");
        DrawText(0, 27, "========================================================================================================================");
    }
    else if (battleResult == "탈출")
    {
        DrawText(18, 13, "전장을 벗어났습니다. 체력을 잃고 맵으로 돌아갑니다.", "yellow");
        DrawText(18, 15, "계속하려면 아무 곳이나 클릭하세요.");
        DrawText(0, 27, "========================================================================================================================");
    }
}

void BattleResultView::DisplayRewardApplied(Player* player, const BattleRewardOption& reward)
{
    DrawText(0, 24, "                                                                                                                        ");
    DrawText(18, 24, std::format("{} 보상이 적용되었습니다.", reward.title), "yellow");
    DrawText(18, 25, std::format("현재 상태: Level {} / HP {}/{} / ATK {} / GOLD : {}", player->GetLevel(), player->Status->GetCurrentHp(), player->Status->GetMaxHp(), player->GetBaseDamage(), player->GetGold()), "cyan");
    DrawText(18, 26, "계속하려면 아무 곳이나 클릭하세요.");
}

void BattleResultView::DisplayBossClear(Player* player, const std::vector<Enemy*>& enemies)
{
    std::string bossName = "Joker King";
    for (const auto& enemy : enemies)
    {
        if (enemy != nullptr)
        {
            bossName = enemy->GetName();
            break;
        }
    }

    DrawText(0, 0, "========================================================================================================================");
    DrawText(51, 1, "BOSS DEFEATED", "yellow");
    DrawText(0, 2, "========================================================================================================================");
    DrawText(24, 5, std::format("{}의 왕관이 바닥에 떨어졌습니다.", bossName), "cyan");
    DrawText(24, 7, "뒤틀린 카드들은 조용히 제자리로 돌아갑니다.");

    DrawText(18, 10, "+--------------------------------------------------------------------------------+");
    DrawText(18, 11, std::format("| 최종 레벨        {:<61}|", player->GetLevel()));
    DrawText(18, 12, std::format("| 최종 체력        {}/{}", player->Status->GetCurrentHp(), player->Status->GetMaxHp()));
    DrawText(18, 13, std::format("| 최종 공격력      {:<61}|", player->GetBaseDamage()));
    DrawText(18, 14, "+--------------------------------------------------------------------------------+");

    DrawText(36, 18, "당신의 덱은 완성되었습니다.", "yellow");
    DrawButton(BattleResultButtons::BossExit());
    DrawText(0, 27, "========================================================================================================================");
}
