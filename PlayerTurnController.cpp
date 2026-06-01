#include "PlayerTurnController.h"
#include "CombatRuleEffect.h"
#include "CombatView.h"
#include "Enemy.h"
#include "Player.h"
#include "ButtonRenderer.h"
#include "CombatButtons.h"
#include <chrono>
#include <thread>

void PlayerTurnController::StartTurn(CombatContext& context, CombatView& view)
{
    PrepareTurn(context);

    for (auto& enemy : context.enemies)
    {
        enemy->SetNextAction();
    }

    view.DisplayCombatView(context.player, context.enemies);
    view.DisplayBattleInfo(context.player, context.enemies, context.handResult, true);
    view.DisplayCards(context.hand, context.handResult, false);
    if (!context.message.empty())
    {
        PRINT_TEXT(8, 16, context.message, "red");
    }

    DrawPhase(context, view);
    if (!context.bIsBattle) return;

    RerollPhase(context, view);
    if (!context.bIsBattle) return;

    ActionPhase(context, view);
}

void PlayerTurnController::PrepareTurn(CombatContext& context)
{
    context.pokerCard.ResetDeck();
    context.pokerCard.Shuffle();
    context.hand = context.pokerCard.DrawCards(5);
    context.handResult = context.pokerCard.EvaluateHand(context.hand);
    context.fixedCards = { false, false, false, false, false };
    context.message.clear();

    context.player->SetActionPoint(0);
    context.player->SetRerolls(3);
    context.player->Status->SetBlock(0);

    for (CombatRuleEffect* effect : context.ruleEffects)
    {
        if (effect == nullptr) continue;
        effect->OnPlayerTurnStart(context);
    }
    context.message = ConsumeRuleMessages(context);
}

void PlayerTurnController::DrawPhase(CombatContext& context, CombatView& view)
{
    MouseClick click = GET_MOUSE_CLICK();
    if (context.message.empty()) context.message = "[카드 뽑기]를 선택해주세요. ";

    while (context.bIsBattle)
    {
        if (TryEscape(context, view, click)) break;
        if (IsClickedHandConfirm(click)) break;

        PRINT_TEXT(8, 16, context.message, "yellow");
        click = GET_MOUSE_CLICK();
    }

    PRINT_TEXT(8, 16, "                                                                                                                ", "yellow");
    view.DisplayCards(context.hand, context.handResult, true);
    view.DisplayFixedButton(context.hand, context.fixedCards);
}

void PlayerTurnController::RerollPhase(CombatContext& context, CombatView& view)
{
    while (context.bIsBattle)
    {
        MouseClick click = GET_MOUSE_CLICK();
        if (TryEscape(context, view, click)) break;

        context.message = "[ 족보 결정 ]을 선택해주세요. 카드 고정 및 리롤을 사용해 카드를 변경 시킬 수 있습니다.  ";
        if (IsClickedHandConfirm(click)) break;

        for (int i = 0; i < static_cast<int>(context.hand.size()); i++)
        {
            if (ButtonRenderer::IsClicked(CombatButtons::FixedCard(i, context.fixedCards[i]), click))
            {
                context.fixedCards[i] = !context.fixedCards[i];
            }
        }

        if (IsClickedReroll(click))
        {
            if (context.player->GetRerolls() > 0)
            {
                for (int i = 0; i < static_cast<int>(context.hand.size()); i++)
                {
                    if (!context.fixedCards[i])
                    {
                        while (true)
                        {
                            Card card = context.pokerCard.DrawCard();
                            if (context.hand[i] != card)
                            {
                                context.hand[i] = card;
                                break;
                            }
                        }
                    }
                }

                context.player->UseReroll();
                for (CombatRuleEffect* effect : context.ruleEffects)
                {
                    if (effect == nullptr) continue;
                    effect->OnPlayerReroll(context);
                }

                std::string ruleMessage = ConsumeRuleMessages(context);
                if (!ruleMessage.empty()) context.message = ruleMessage;
                if (!context.player->Status->IsAlive())
                {
                    context.bIsVictory = false;
                    context.battleResult = "패배";
                    context.bIsBattle = false;
                    break;
                }

                context.handResult = context.pokerCard.EvaluateHand(context.hand);
                view.DisplayCards(context.hand, context.handResult, true);
                view.DisplayBattleInfo(context.player, context.enemies, context.handResult, true);
            }
            else
            {
                context.message = "리롤 횟수가 부족합니다.                                                                           ";
            }
        }

        view.DisplayFixedButton(context.hand, context.fixedCards);
        PRINT_TEXT(8, 16, context.message, "yellow");
    }

    if (!context.bIsBattle) return;

    context.player->SetActionPoint(context.handResult.actionPoint);
    PRINT_TEXT(8, 16, std::format("[ 족보 결정 성공 ! ] : 행동력 {} 획득 !                                                   ", context.handResult.actionPoint), "yellow");
    view.DisplayCards(context.hand, context.handResult, true);
    view.DisplayBattleInfo(context.player, context.enemies, context.handResult, true);

    for (CombatRuleEffect* effect : context.ruleEffects)
    {
        if (effect == nullptr) continue;
        effect->OnHandConfirmed(context);
    }

    std::string ruleMessage = ConsumeRuleMessages(context);
    if (!ruleMessage.empty())
    {
        PRINT_TEXT(8, 16, "                                                                                                                ");
        PRINT_TEXT(8, 16, ruleMessage, "red");
        view.DisplayBattleInfo(context.player, context.enemies, context.handResult, true);
        if (!context.player->Status->IsAlive())
        {
            context.bIsVictory = false;
            context.battleResult = "패배";
            context.bIsBattle = false;
        }
    }
}

void PlayerTurnController::ActionPhase(CombatContext& context, CombatView& view)
{
    while (context.bIsBattle)
    {
        MouseClick click = GET_MOUSE_CLICK();
        if (TryEscape(context, view, click)) break;

        if (context.player->GetActionPoint() > 0)
        {
            if (IsClickedAttack(click))
            {
                view.DisplayAttackInfo(true);
                click = GET_MOUSE_CLICK();
                view.DisplayAttackInfo(false);

                Enemy* enemy = GetClickedEnemy(click, context.enemies);
                if (enemy != nullptr)
                {
                    bool bWasEnemyAlive = enemy->Status->IsAlive();
                    context.player->DoAttack(enemy, context.handResult.bonusDamage);
                    view.DisplayAttackResult(enemy, context.player->GetBaseDamage() + context.handResult.bonusDamage, true);
                    for (int t = 0; t < 50; t += 10)
                    {
                        PRINT_TEXT(15 + t, 8, "          ");
                        view.DisplaySprite(25 + t, 8, context.player->GetWeaponSprite());
                        std::this_thread::sleep_for(std::chrono::milliseconds(100));
                    }
                    PRINT_TEXT(15 + 50, 8, "          ");

                    if (bWasEnemyAlive && !enemy->Status->IsAlive())
                    {
                        view.DisplayEnemyDiedAnim(enemy);
                        view.DisplayCombatView(context.player, context.enemies);
                        view.DisplayBattleInfo(context.player, context.enemies, context.handResult, true);
                    }

                    if (IsDiedAllEnemy(context.enemies))
                    {
                        PRINT_TEXT(8, 16, "모든 적을 처치했습니다. 아무 곳이나 클릭시 전투 결과가 정산됩니다.                              ", "yellow");
                        GET_MOUSE_CLICK();
                        context.bIsBattle = false;
                        context.bIsVictory = true;
                        context.battleResult = "승리";
                        break;
                    }
                    context.player->SetActionPoint(context.player->GetActionPoint() - 1);
                }
            }
            else if (IsClickedBlock(click))
            {
                context.player->GainBlock(3 + context.handResult.bonusBlock);
                PRINT_TEXT(8, 16, "                                                                                                                ");
                PRINT_TEXT(8, 16, std::format("{} 방어막 획득 !    ", context.player->Status->GetBlock()), "yellow");
                context.player->SetActionPoint(context.player->GetActionPoint() - 1);
            }
            else
            {
                PRINT_TEXT(8, 16, "                                                                                                                ");
                PRINT_TEXT(8, 16, "[ 공격 ] / [ 방어 ] / [ 탈출 ] / [ 턴 종료 ] 버튼을 선택하세요. ", "yellow");
            }
        }
        else
        {
            PRINT_TEXT(8, 16, "                                                                                                                ");
            PRINT_TEXT(8, 16, "행동력이 부족합니다. [ 턴 종료 ] 버튼을 선택하세요.", "yellow");
        }

        if (IsClickedTurnEnd(click))
        {
            break;
        }

        view.DisplayCombatView(context.player, context.enemies);
        view.DisplayBattleInfo(context.player, context.enemies, context.handResult, true);
    }
}

bool PlayerTurnController::TryEscape(CombatContext& context, CombatView& view, MouseClick click)
{
    if (!ButtonRenderer::IsClicked(CombatButtons::Escape(), click)) return false;

    view.DisplayEscapeInfo(true);
    MouseClick confirmClick = GET_MOUSE_CLICK();
    if (ButtonRenderer::IsClicked(CombatButtons::EscapeConfirmYes(), confirmClick))
    {
        context.bIsVictory = false;
        context.battleResult = "탈출";
        context.bIsBattle = false;
        return true;
    }

    view.DisplayEscapeInfo(false);
    return false;
}

bool PlayerTurnController::IsClickedAttack(MouseClick click) const
{
    return ButtonRenderer::IsClicked(CombatButtons::Attack(), click);
}

bool PlayerTurnController::IsClickedBlock(MouseClick click) const
{
    return ButtonRenderer::IsClicked(CombatButtons::Block(), click);
}

bool PlayerTurnController::IsClickedReroll(MouseClick click) const
{
    return ButtonRenderer::IsClicked(CombatButtons::Reroll(), click);
}

bool PlayerTurnController::IsClickedHandConfirm(MouseClick click) const
{
    return ButtonRenderer::IsClicked(CombatButtons::ConfirmHand(), click);
}

bool PlayerTurnController::IsClickedTurnEnd(MouseClick click) const
{
    return ButtonRenderer::IsClicked(CombatButtons::TurnEnd(), click);
}

bool PlayerTurnController::IsDiedAllEnemy(const std::vector<Enemy*>& enemies) const
{
    int diedCount = 0;
    for (const auto& enemy : enemies)
    {
        if (enemy != nullptr && !enemy->Status->IsAlive()) diedCount++;
    }
    return diedCount == static_cast<int>(enemies.size());
}

Enemy* PlayerTurnController::GetClickedEnemy(MouseClick click, const std::vector<Enemy*>& enemies) const
{
    for (Enemy* enemy : enemies)
    {
        if (enemy == nullptr) continue;
        Rect* rect = enemy->GetSpriteRect();
        if (enemy->Status->IsAlive() && IS_IN_AREA(click, rect->left, rect->top, rect->right, rect->bottom))
        {
            return enemy;
        }
    }
    return nullptr;
}

std::string PlayerTurnController::ConsumeRuleMessages(const CombatContext& context) const
{
    std::string result;
    for (CombatRuleEffect* effect : context.ruleEffects)
    {
        if (effect == nullptr) continue;
        std::string message = effect->GetLastMessage();
        if (message.empty()) continue;
        if (!result.empty()) result.append(" ");
        result.append(message);
    }
    return result;
}
