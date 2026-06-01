#pragma once
#include <Windows.h>
#include <conio.h>
#include "Utils.h"
#include "Instance.h"

struct MouseClick
{
	int x;
	int y;
};

class ConsoleFramework : public Instance<ConsoleFramework>
{
	friend class Instance<ConsoleFramework>;
private:
	HANDLE hConsole;
	HANDLE hInput;

	ConsoleFramework();
	~ConsoleFramework();
public:

	void Init();
	void ClearScreen();
	void HideCursor();
	void ShowCursor();
	void GotoXY(int x, int y);
	char GetKey();
	MouseClick GetMouseClick();
	bool IsInArea(const MouseClick& click, int left, int top, int right, int bottom) const;
	void PrintText(int x, int y, const std::string& text, const std::string& color = "white");
    void PrintTextToSlow(int x, int y, const std::string& text, const std::string& color = "white", int delay = 5);
    void SetTextColor(const std::string& color);
};

#define CONSOLE (ConsoleFramework::GetInstance())
#define CLEAR_SCREEN() CONSOLE->ClearScreen()
#define GET_MOUSE_CLICK() CONSOLE->GetMouseClick()
#define IS_IN_AREA(...) CONSOLE->IsInArea(__VA_ARGS__)
#define PRINT_TEXT(...) CONSOLE->PrintText(__VA_ARGS__)
#define PRINT_TEXT_SLOW(...) CONSOLE->PrintTextToSlow(__VA_ARGS__)
