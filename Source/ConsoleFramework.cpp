#include <thread>
#include <chrono>
#include "ConsoleFramework.h"
#include "Logger.h"
ConsoleFramework::ConsoleFramework()
{
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	hInput = GetStdHandle(STD_INPUT_HANDLE);
	std::ios::sync_with_stdio(false);
	std::cin.tie(nullptr);
	std::cout.tie(nullptr);
	SetConsoleOutputCP(CP_UTF8);
}

ConsoleFramework::~ConsoleFramework() {};

void ConsoleFramework::Init()
{
	SetConsoleTitleA("Adventure RPG");
	SMALL_RECT windowSize = { 0, 0, 120, 30 };
	SetConsoleWindowInfo(hConsole, TRUE, &windowSize);

	DWORD inputMode = 0;
	GetConsoleMode(hInput, &inputMode);
	inputMode &= ~ENABLE_QUICK_EDIT_MODE;
	inputMode |= ENABLE_EXTENDED_FLAGS | ENABLE_MOUSE_INPUT;
	SetConsoleMode(hInput, inputMode);
	HideCursor();
}

void ConsoleFramework::ClearScreen()
{
	COORD coord = { 0, 0 };
	CONSOLE_SCREEN_BUFFER_INFO bufferInfo;
	GetConsoleScreenBufferInfo(hConsole, &bufferInfo);
	DWORD consoleSize = bufferInfo.dwSize.X * bufferInfo.dwSize.Y;
	DWORD written;
	FillConsoleOutputCharacter(hConsole, ' ', consoleSize, coord, &written);
	FillConsoleOutputAttribute(hConsole, bufferInfo.wAttributes, consoleSize, coord, &written);
	SetConsoleCursorPosition(hConsole, coord);
    std::cout << "\033[2J\033[3J\033[1;1H";
}

void ConsoleFramework::HideCursor()
{
	CONSOLE_CURSOR_INFO cursorInfo;
	GetConsoleCursorInfo(hConsole, &cursorInfo);
	cursorInfo.bVisible = FALSE;
	SetConsoleCursorInfo(hConsole, &cursorInfo);
}

void ConsoleFramework::ShowCursor()
{
	CONSOLE_CURSOR_INFO cursorInfo;
	GetConsoleCursorInfo(hConsole, &cursorInfo);
	cursorInfo.bVisible = TRUE;
	SetConsoleCursorInfo(hConsole, &cursorInfo);
}

void ConsoleFramework::GotoXY(int x, int y)
{
	COORD coord;
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(hConsole, coord);
}

char ConsoleFramework::GetKey()
{
	char key = _getch();
	ADD_LOG(LogType::KeyEvent, std::format("Key pressed: {}", key));
	return key;
}

MouseClick ConsoleFramework::GetMouseClick()
{
	INPUT_RECORD inputRecord;
	DWORD eventCount = 0;

	while (true)
	{
		ReadConsoleInput(hInput, &inputRecord, 1, &eventCount);
		if (inputRecord.EventType == MOUSE_EVENT)
		{
			const MOUSE_EVENT_RECORD& mouseEvent = inputRecord.Event.MouseEvent;
			if (mouseEvent.dwEventFlags == 0 && (mouseEvent.dwButtonState & FROM_LEFT_1ST_BUTTON_PRESSED))
			{
				MouseClick click{ mouseEvent.dwMousePosition.X, mouseEvent.dwMousePosition.Y };
				ADD_LOG(LogType::KeyEvent, std::format("Mouse clicked: x : {}, y : {}", click.x, click.y));
				return click;
			}
		}
	}
}

bool ConsoleFramework::IsInArea(const MouseClick& click, int left, int top, int right, int bottom) const
{
	return click.x >= left && click.x <= right && click.y >= top && click.y <= bottom;
}

void ConsoleFramework::PrintText(int x, int y, const std::string& text, const std::string& color)
{

	GotoXY(x, y);
    SetTextColor(color);
    std::cout << text;
   
}

void ConsoleFramework::PrintTextToSlow(int x, int y, const std::string& text, const std::string& color, int delay)
{
    GotoXY(x, y);
    SetTextColor(color);
    for (auto& c : text)
    {
        if (c != ' ') std::this_thread::sleep_for(std::chrono::milliseconds(delay));
        std::cout << c;
    }
}

void ConsoleFramework::SetTextColor(const std::string& color)
{
    if (color == "red")
        std::cout << "\033[31m";
    else if (color == "blue")
        std::cout << "\033[34m";
    else if (color == "green")
        std::cout << "\033[32m";
    else if (color == "yellow")
        std::cout << "\033[33m";
    else if (color == "cyan")
        std::cout << "\033[36m";
    else if (color == "white")
        std::cout << "\033[37m";
    else if (color == "black")
        std::cout << "\033[30m";
    else
        std::cout << "\033[0m";
}