#pragma once
#include "Utils.h"

class MainMenu
{
private:
	std::string MainMenuText;
public:
	MainMenu();
	~MainMenu();

	std::string GetMainMenuText() const;
	std::string GetMenuOption(int select);
};
