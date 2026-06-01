#include "UIManager.h"
#include "ConsoleFramework.h"

#include "MainMenu.h"
#include "MapView.h"
#include "Player.h"
#include "ButtonRenderer.h"
#include "MainMenuButtons.h"
#include "RestButtons.h"

UIManager::UIManager()
{
	mainMenu = std::make_unique<MainMenu>();
	mapView = std::make_unique<MapView>();
	
}

UIManager::~UIManager()
{
}

void UIManager::DisplayMainMenu()
{
	CLEAR_SCREEN();
	PRINT_TEXT(0, 0, mainMenu->GetMainMenuText(), "white");
	PRINT_TEXT(47, 9, "TEXT POKER BATTLE", "yellow");
    ButtonRenderer::Draw(MainMenuButtons::Start());
    ButtonRenderer::Draw(MainMenuButtons::Rules());
    ButtonRenderer::Draw(MainMenuButtons::Exit());
}

void UIManager::DisplayGameRules()
{
    PRINT_TEXT(0, 0, "========================================================================================================================", "white");
    PRINT_TEXT(44, 3, "게임 설명", "yellow");
    PRINT_TEXT(24, 7, "전투는 턴제로 진행되며, 플레이어가 먼저 행동합니다.");
    PRINT_TEXT(24, 9, "매 턴 포커 카드 5장을 뽑고, 족보에 따라 공격/방어 보너스를 얻습니다.");
    PRINT_TEXT(24, 11, "전투 중 공격, 방어, 탈출 버튼을 마우스로 클릭해 선택할 수 있습니다.");
    PRINT_TEXT(0, 18, "========================================================================================================================", "white");
    PRINT_TEXT(38, 22, "[ 메인 메뉴로 돌아가기 ]", "cyan");
}

void UIManager::DisplayRestUI(Player* player)
{
    CLEAR_SCREEN();

    PRINT_TEXT(0, 0, "========================================================================================================================", "white");
    PRINT_TEXT(52, 2, "휴식 공간", "yellow");
    PRINT_TEXT(0, 4, "========================================================================================================================", "white");

    PRINT_TEXT(47, 7, "        )   (        ", "yellow");
    PRINT_TEXT(47, 8, "       (     )       ", "yellow");
    PRINT_TEXT(47, 9, "        )   (        ", "yellow");
    PRINT_TEXT(47, 10, "      /=======\\      ", "red");
    PRINT_TEXT(47, 11, "     /   🔥    \\     ", "red");
    PRINT_TEXT(47, 12, "    /___________\\    ", "red");
    PRINT_TEXT(47, 13, "       /     \\       ", "white");
    PRINT_TEXT(47, 14, "      /_______\\      ", "white");

    if (player != nullptr)
    {
        PRINT_TEXT(41, 17, std::format("HP: {}/{}   공격력: {}", player->Status->GetCurrentHp(), player->Status->GetMaxHp(), player->GetBaseDamage()), "cyan");
    }

    ButtonRenderer::Draw(RestButtons::Rest());
    ButtonRenderer::Draw(RestButtons::AbilityUpgrade());

    PRINT_TEXT(32, 27, "체력을 회복합니다.", "green");
    PRINT_TEXT(83, 27, "공격력이 증가합니다.", "cyan");
}

void UIManager::DisplayRestResult(Player* player, const std::string& resultText)
{
    PRINT_TEXT(0, 29, "========================================================================================================================", "white");
    PRINT_TEXT(34, 20, "                                                        ");
    PRINT_TEXT(34, 20, resultText, "yellow");

    if (player != nullptr)
    {
        PRINT_TEXT(41, 17, "                                                        ");
        PRINT_TEXT(41, 17, std::format("HP: {}/{}   공격력: {}", player->Status->GetCurrentHp(), player->Status->GetMaxHp(), player->GetBaseDamage()), "cyan");
    }

    PRINT_TEXT(37, 21, "클릭하면 맵으로 돌아갑니다.", "white");
}

bool UIManager::IsClickedRestButton(const MouseClick& click)
{
    return ButtonRenderer::IsClicked(RestButtons::Rest(), click);
}

bool UIManager::IsClickedAbilityUpgradeButton(const MouseClick& click)
{
    return ButtonRenderer::IsClicked(RestButtons::AbilityUpgrade(), click);
}
