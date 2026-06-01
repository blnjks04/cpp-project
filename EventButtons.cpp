#include "EventButtons.h"
#include "ButtonFactory.h"
#include <format>

Button EventButtons::Choice(int index, int y, const std::string& text)
{
    return ButtonFactory::Text(ButtonId::EventChoice, 12, y, std::format("[ {} ] {}", index + 1, text), 97, "cyan");
}

std::vector<Button> EventButtons::PokerBetButtons()
{
    return {
        ButtonFactory::Text(ButtonId::PokerBetHp5, 2, 26, "[ HP 5 ]", 8),
        ButtonFactory::Text(ButtonId::PokerBetHp10, 13, 26, "[ HP 10 ]", 9),
        ButtonFactory::Text(ButtonId::PokerBetAttack1, 25, 26, "[ 공격 1 ]", 10),
        ButtonFactory::Text(ButtonId::PokerBetAttack2, 38, 26, "[ 공격 2 ]", 10),
        ButtonFactory::Text(ButtonId::PokerCheck, 51, 26, "[ 체크 ]", 8)
    };
}

Button EventButtons::PokerNext(bool bIsResult)
{
    std::string text = bIsResult ? "[ 결과 확인 ]" : "[ 카드 받기 ]";
    Button button = ButtonFactory::Text(ButtonId::PokerNext, 62, 26, text, 12);
    button.lines.insert(button.lines.begin(), { 2, 26, "                                                                          ", "white" });
    return button;
}
