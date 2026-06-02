#include "EnemyTurnController.h"
#include "CombatRuleEffect.h"
#include "CombatView.h"
#include "ConsoleFramework.h"
#include "Enemy.h"
#include "Player.h"
#include <chrono>
#include <thread>

namespace
{
    bool IsDiedAllEnemy(const std::vector<Enemy*>& enemies)
    {
        int diedCount = 0;
        for (const auto& enemy : enemies)
        {
            if (enemy != nullptr && !enemy->Status->IsAlive()) diedCount++;
        }
        return diedCount == static_cast<int>(enemies.size());
    }
}

void EnemyTurnController::StartTurn(CombatContext& context, CombatView& view)
{
    view.DisplayBattleInfo(context.player, context.enemies, context.handResult, false);
    if (IsDiedAllEnemy(context.enemies))
    {
        view.DisplayMessage("모든 적을 처치했습니다. 아무 곳이나 클릭시 전투 결과가 정산됩니다.");
        GET_MOUSE_CLICK();
        context.bIsBattle = false;
        context.bIsVictory = true;
        context.battleResult = "승리";
        return;
    }

    for (CombatRuleEffect* effect : context.ruleEffects)
    {
        if (effect == nullptr) continue;
        effect->OnEnemyTurnStart(context);
    }

    for (auto& enemy : context.enemies)
    {
        if (enemy == nullptr || !enemy->Status->IsAlive()) continue;

        enemy->ExecuteFSM(context.player);
        view.DisplayMessage(std::format("{} 플레이어 남은 체력 : {}", enemy->GetLastActionMessage(), context.player->Status->GetCurrentHp()));
        std::this_thread::sleep_for(std::chrono::milliseconds(3000));

        if (!context.player->Status->IsAlive())
        {
            context.bIsBattle = false;
            context.bIsVictory = false;
            context.battleResult = "패배";
            return;
        }
    }

    view.DisplayMessage("아무 곳이나 클릭시 다음 턴을 진행합니다.");
    GET_MOUSE_CLICK();
    view.ClearMessageBuffer();
}
