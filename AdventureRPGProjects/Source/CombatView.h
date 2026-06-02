#pragma once
#include "Utils.h"
#include "PokerCard.h"
#include "View.h"

class Player;
class Enemy;

class CombatView : public View
{
private:
    PokerCard pokerCard;

public:
    CombatView();
    virtual ~CombatView();

    void DisplayCombatView(Player* player, std::vector<Enemy*>& enemies);
    void DisplayCards(const std::vector<Card>& hand, const PokerHandResult& handResult, bool bIsDisplay);
    void DisplayFixedButton(const std::vector<Card>& hand, const std::vector<bool>& fixedCards = {});
    void DisplayBattleInfo(Player* player, std::vector<Enemy*>& enemies, const PokerHandResult& handResult, bool isPlayerTurn);
    void DisplayEscapeInfo(bool bIsEscape);
    void DisplayAttackInfo(bool bIsDisplay);
    void DisplayAttackResult(Enemy* enemy, int damage, bool bIsDisplay);
    void DisplayEnemyDiedAnim(Enemy* enemy);
    void DisplaySprite(int x, int y, const std::vector<std::string>& sprite, const std::string& color = "white");
    void ClearMessageBuffer();
    void DisplayMessage(const std::string& message, const std::string& color = "yellow");
};
