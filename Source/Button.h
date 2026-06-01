#pragma once
#include <string>
#include <vector>

enum class ButtonId
{
    None,
    MainStart,
    MainRules,
    MainExit,
    Rest,
    AbilityUpgrade,
    DrawCards,
    ConfirmHand,
    FixedCard,
    Attack,
    Block,
    Reroll,
    TurnEnd,
    Escape,
    EscapeConfirmYes,
    RewardOption,
    BossExit,
    EventChoice,
    PokerBetHp5,
    PokerBetHp10,
    PokerBetAttack1,
    PokerBetAttack2,
    PokerCheck,
    PokerNext
};

struct ButtonRect
{
    int left = 0;
    int top = 0;
    int right = 0;
    int bottom = 0;
};

struct ButtonLine
{
    int x = 0;
    int y = 0;
    std::string text;
    std::string color = "white";
};

struct Button
{
    ButtonId id = ButtonId::None;
    ButtonRect rect;
    std::vector<ButtonLine> lines;
};
