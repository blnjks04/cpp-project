#pragma once
#include "Utils.h"

class Character;
class MainMenu;
class MapView;
class Player;
struct MouseClick;

class UIManager
{
public:
	UIManager();
	~UIManager();

	std::unique_ptr<MainMenu> mainMenu;
	std::unique_ptr<MapView> mapView;
	void DisplayMainMenu();
    void DisplayGameRules();
	void DisplayPlayerStatus(Player* player);
	void DisplayBattleUI(Player* player, Character* enemy);
	void DisplayEventUI(const std::string& eventDescription);
	void DisplayResultUI(bool isVictory);
    void DisplayRestUI(Player* player);
    void DisplayRestResult(Player* player, const std::string& resultText);
    bool IsClickedRestButton(const MouseClick& click);
    bool IsClickedAbilityUpgradeButton(const MouseClick& click);
};
