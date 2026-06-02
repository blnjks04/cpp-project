#include "BattleResultButtons.h"
#include "ButtonFactory.h"
#include <format>

Button BattleResultButtons::RewardOption(int index, const std::string& title, const std::string& description)
{
    int y = 14 + (index * 4);
    return ButtonFactory::MultiLine(
        ButtonId::RewardOption,
        22,
        y,
        75,
        3,
        {
            { 22, y, std::format("[ {} ] {}", index + 1, title), "cyan" },
            { 28, y + 1, description, "white" }
        });
}

Button BattleResultButtons::BossExit()
{
    return ButtonFactory::Text(ButtonId::BossExit, 45, 23, "[ 게임 종료 ]", 13, "red");
}
