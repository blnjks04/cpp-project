#include "CombatView.h"
#include "CombatButtons.h"
#include "ConsoleFramework.h"
#include "Logger.h"
#include <chrono>
#include <thread>

#include "Enemy.h"
#include "BossEnemy.h"
#include "Player.h"

CombatView::CombatView()
{
}

CombatView::~CombatView()
{
}

void CombatView::DisplayCombatView(Player* player, std::vector<Enemy*>& enemies)
{
    PRINT_TEXT(0, 0, "========================================================================================================================");
    PRINT_TEXT(2, 1, "BATTLE");
    PRINT_TEXT(0, 2, "========================================================================================================================");
    PRINT_TEXT(0, 14, "========================================================================================================================");
    PRINT_TEXT(0, 17, "========================================================================================================================");
    PRINT_TEXT(0, 26, "========================================================================================================================");
    PRINT_TEXT(0, 29, "========================================================================================================================");
    PRINT_TEXT(2, 16, "안내:");

    PRINT_TEXT(2, 3, "PLAYER");
    PRINT_TEXT(2, 4, std::format("[ {} ]", player->GetName()), "blue");
    PRINT_TEXT(2, 5, std::format("HP: {}/{}  BLOCK: {}", player->Status->GetCurrentHp(), player->Status->GetMaxHp(), player->Status->GetBlock()), "blue");
    DisplaySprite(8, 7, player->GetSprite());
    DisplaySprite(15, 8, player->GetWeaponSprite());
    for (auto& enemy : enemies)
    {
        Rect* rect = enemy->GetSpriteRect();
        ADD_LOG(LogType::ALL, std::format("{} {}", rect->left, rect->top));
        BossEnemy* bossEnemy = dynamic_cast<BossEnemy*>(enemy);
        std::string spriteColor = (bossEnemy != nullptr && bossEnemy->IsPhase2()) ? "red" : "white";
        DisplaySprite(rect->left, rect->top, enemy->GetSprite(), spriteColor);
        PRINT_TEXT(rect->left - 2, rect->bottom, std::format("[ {} ] Action : {}", enemy->GetName(), enemy->GetNextAction()), "red");
        PRINT_TEXT(rect->left - 2, rect->bottom + 1, std::format("HP: {}/{}  BLOCK: {}       ", enemy->Status->GetCurrentHp(), enemy->Status->GetMaxHp(), enemy->Status->GetBlock()), "red");
    }
}

void CombatView::DisplayCards(const std::vector<Card>& hand, const PokerHandResult& handResult, bool bIsDisplay)
{
    for (int i = 0; i < hand.size(); i++)
    {
        int x = 18 + (i * 10);
        int y = 18;
        if (bIsDisplay) pokerCard.DisplayCard(x, y, hand[i]);
        else pokerCard.DisplayHiddenCard(x, y);
    }

    PRINT_TEXT(2, 15, "현재 족보:                     ");
    if (!bIsDisplay) DrawButton(CombatButtons::DrawCards());
    else
    {
        DrawButton(CombatButtons::ConfirmHand());
        PRINT_TEXT(2, 15, std::format("현재 족보: {}", handResult.name));
        PRINT_TEXT(40, 15, std::format("공격 보너스 + {}   방어 보너스 + {}  ", handResult.bonusDamage, handResult.bonusBlock));
    }
}

void CombatView::DisplayFixedButton(const std::vector<Card>& hand, const std::vector<bool>& fixedCards)
{
    for (int i = 0; i < hand.size(); i++)
    {
        bool isFixed = i < fixedCards.size() && fixedCards[i];
        // 카드 고정 버튼은 전투 화면 전용 버튼 정의를 사용한다.
        DrawButton(CombatButtons::FixedCard(i, isFixed));
    }
}

void CombatView::DisplayBattleInfo(Player* player, std::vector<Enemy*>& enemies, const PokerHandResult& handResult, bool isPlayerTurn)
{
    if (isPlayerTurn)
    {
        PRINT_TEXT(54, 1, "내 턴");
        PRINT_TEXT(84, 15, std::format("행동력 {} / 리롤 {}", player->GetActionPoint(), player->GetRerolls()));
        // 전투 행동 버튼 출력과 클릭 영역은 CombatButtons 정의로 맞춘다.
        DrawButton(CombatButtons::Attack(player->GetBaseDamage() + handResult.bonusDamage));
        DrawButton(CombatButtons::Block(3 + handResult.bonusBlock));
        DrawButton(CombatButtons::Reroll());
        DrawButton(CombatButtons::TurnEnd());
        DrawButton(CombatButtons::Escape());
    }
    else
    {
        PRINT_TEXT(54, 1, "적 턴", "red");
        PRINT_TEXT(84, 1, "                         ");
        PRINT_TEXT(2, 28, "                                                                                                    ");
        PRINT_TEXT(70, 28, "[ 턴 종료 ]", "black");
    }
}

void CombatView::DisplayEscapeInfo(bool bIsEscape)
{
    std::string color = "red";
    if (!bIsEscape) color = "black";
    ClearMessageBuffer();
    PRINT_TEXT(8, 16, std::format("탈출 선택 시 최대 체력의 50%를 잃으며, 맵으로 이동합니다. 정말로 탈출 하시겠습니까 ? [예] / [아니오]"), color);
}

void CombatView::DisplayAttackInfo(bool bIsDisplay)
{
    std::string color = "yellow";
    if (!bIsDisplay) color = "black";
    ClearMessageBuffer();
    PRINT_TEXT(8, 16, "공격 할 적을 클릭하세요. ", color);
}

void CombatView::DisplayAttackResult(Enemy* enemy, int damage, bool bIsDisplay)
{
    std::string color = "yellow";
    if (!bIsDisplay) color = "black";
    ClearMessageBuffer();
    PRINT_TEXT(8, 16, std::format("{}에게 {}의 피해를 입혔습니다.", enemy->GetName(), damage), "yellow");
}

void CombatView::DisplayEnemyDiedAnim(Enemy* enemy)
{
    if (enemy == nullptr) return;

    Rect* rect = enemy->GetSpriteRect();
    std::vector<std::vector<std::string>>& diedAnim = enemy->GetAnimSprite("diedAnim");
    if (rect == nullptr || diedAnim.empty()) return;

    for (const auto& frame : diedAnim)
    {
        DisplaySprite(rect->left, rect->top, frame);
        std::this_thread::sleep_for(std::chrono::milliseconds(180));
    }

    enemy->SetSprite(diedAnim.back());
}

void CombatView::DisplaySprite(int x, int y, const std::vector<std::string>& sprite, const std::string& color)
{
    for (int i = 0; i < sprite.size(); i++)
    {
        PRINT_TEXT(x, y + i, sprite[i], color);
    }
}

void CombatView::ClearMessageBuffer()
{
    PRINT_TEXT(8, 16, std::format("{:<90}", ""));
}

void CombatView::DisplayMessage(const std::string& message, const std::string& color)
{
    ClearMessageBuffer();
    PRINT_TEXT(8, 16, message, color);
}