#pragma once
#include "Button.h"

class EventButtons
{
public:
    static Button Choice(int index, int y, const std::string& text);
    static std::vector<Button> PokerBetButtons();
    static Button PokerNext(bool bIsResult);
};
