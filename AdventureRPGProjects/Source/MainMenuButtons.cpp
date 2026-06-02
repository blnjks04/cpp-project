#include "MainMenuButtons.h"
#include "ButtonFactory.h"

Button MainMenuButtons::Start()
{
    return ButtonFactory::Text(ButtonId::MainStart, 48, 13, "[ 게임 시작 ]", 15, "cyan");
}

Button MainMenuButtons::Rules()
{
    return ButtonFactory::Text(ButtonId::MainRules, 48, 15, "[ 게임 설명 ]", 15, "green");
}

Button MainMenuButtons::Exit()
{
    return ButtonFactory::Text(ButtonId::MainExit, 48, 17, "[ 종료 ]", 8, "red");
}
