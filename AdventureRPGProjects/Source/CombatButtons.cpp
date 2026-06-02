#include "CombatButtons.h"
#include "ButtonFactory.h"
#include <format>

Button CombatButtons::DrawCards()
{
    return ButtonFactory::Text(ButtonId::DrawCards, 2, 21, "[ 카드 뽑기 ]", 13);
}

Button CombatButtons::ConfirmHand()
{
    return ButtonFactory::Text(ButtonId::ConfirmHand, 2, 21, "[ 족보 결정 ]", 13);
}

Button CombatButtons::FixedCard(int index, bool bIsFixed)
{
    int x = 18 + (index * 10);
    std::string text = bIsFixed ? "[ 고정됨 ]" : " [ 고정 ] ";
    std::string color = bIsFixed ? "yellow" : "white";
    return ButtonFactory::Text(ButtonId::FixedCard, x, 25, text, 9, color);
}

Button CombatButtons::Attack(int damage)
{
    std::string text = damage > 0 ? std::format("[ 공격 : {} 피해 ]", damage) : "[ 공격 ]";
    return ButtonFactory::Text(ButtonId::Attack, 2, 28, text, 22, "cyan");
}

Button CombatButtons::Block(int block)
{
    std::string text = block > 0 ? std::format("[ 방어 : {} 방어막 ]", block) : "[ 방어 ]";
    return ButtonFactory::Text(ButtonId::Block, 28, 28, text, 25, "green");
}

Button CombatButtons::Reroll()
{
    return ButtonFactory::Text(ButtonId::Reroll, 56, 28, "[ 리롤 ]", 8, "yellow");
}

Button CombatButtons::TurnEnd()
{
    return ButtonFactory::Text(ButtonId::TurnEnd, 70, 28, "[ 턴 종료 ]", 11);
}

Button CombatButtons::Escape()
{
    return ButtonFactory::Text(ButtonId::Escape, 87, 28, "[ 탈출 ]", 8, "red");
}

Button CombatButtons::EscapeConfirmYes()
{
    return ButtonFactory::MultiLine(ButtonId::EscapeConfirmYes, 94, 16, 4, 1, {});
}
