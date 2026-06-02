#pragma once
#include "Button.h"

class ButtonFactory
{
public:
    static Button Text(ButtonId id, int x, int y, const std::string& text, int width, const std::string& color = "white");
    static Button Box(ButtonId id, int x, int y, int width, const std::string& label, const std::string& color = "white");
    static Button MultiLine(ButtonId id, int x, int y, int width, int height, const std::vector<ButtonLine>& lines);

private:
    static std::string PadRight(const std::string& text, int width);
    static std::string CenterInBox(const std::string& text, int innerWidth);
};
