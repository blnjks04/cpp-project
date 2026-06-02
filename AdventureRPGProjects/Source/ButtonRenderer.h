#pragma once
#include "Button.h"
#include "ConsoleFramework.h"

class ButtonRenderer
{
public:
    static void Draw(const Button& button);
    static void DrawAll(const std::vector<Button>& buttons);
    static bool IsClicked(const Button& button, const MouseClick& click);
    static int GetClickedIndex(const std::vector<Button>& buttons, const MouseClick& click);
};
