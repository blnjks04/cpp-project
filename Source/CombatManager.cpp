#include "CombatManager.h"
#include "BattleResultController.h"
#include "BossEnemy.h"
#include "BossPhase2RuleEffect.h"
#include "CombatContext.h"
#include "CombatRuleEffect.h"
#include "CombatView.h"
#include "Enemy.h"
#include "EnemyTurnController.h"
#include "GameStateManager.h"
#include "Logger.h"
#include "PlayerTurnController.h"

void CombatManager::StartBattle(Player* player, std::vector<Enemy*>& enemies)
{
    if (player == nullptr) return;
    for (Enemy* enemy : enemies)
    {
        if (enemy == nullptr) return;
    }

    ADD_LOG(LogType::BattleEvent, "전투 시작");

    CombatContext context;
    context.player = player;
    context.enemies = enemies;
    context.battleState = CURRENT_STATE();

    std::vector<std::unique_ptr<CombatRuleEffect>> ruleStorage;
    for (Enemy* enemy : enemies)
    {
        BossEnemy* bossEnemy = dynamic_cast<BossEnemy*>(enemy);
        if (bossEnemy == nullptr) continue;

        ruleStorage.push_back(std::make_unique<BossPhase2RuleEffect>(bossEnemy));
        context.ruleEffects.push_back(ruleStorage.back().get());
    }

    CombatView combatView;
    PlayerTurnController playerTurnController;
    EnemyTurnController enemyTurnController;
    BattleResultController resultController;

    while (context.bIsBattle)
    {
        playerTurnController.StartTurn(context, combatView);
        if (!context.bIsBattle) break;

        enemyTurnController.StartTurn(context, combatView);
    }

    resultController.Resolve(context);
}
