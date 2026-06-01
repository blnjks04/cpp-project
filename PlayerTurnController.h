#pragma once
#include "ConsoleFramework.h"
#include "CombatContext.h"

class CombatView;
class Enemy;

class PlayerTurnController
{
public:
    void StartTurn(CombatContext& context, CombatView& view);

private:
    void PrepareTurn(CombatContext& context);
    void DrawPhase(CombatContext& context, CombatView& view);
    void RerollPhase(CombatContext& context, CombatView& view);
    void ActionPhase(CombatContext& context, CombatView& view);
    bool TryEscape(CombatContext& context, CombatView& view, MouseClick click);
    bool IsClickedAttack(MouseClick click) const;
    bool IsClickedBlock(MouseClick click) const;
    bool IsClickedReroll(MouseClick click) const;
    bool IsClickedHandConfirm(MouseClick click) const;
    bool IsClickedTurnEnd(MouseClick click) const;
    bool IsDiedAllEnemy(const std::vector<Enemy*>& enemies) const;
    Enemy* GetClickedEnemy(MouseClick click, const std::vector<Enemy*>& enemies) const;
    std::string ConsumeRuleMessages(const CombatContext& context) const;
};
