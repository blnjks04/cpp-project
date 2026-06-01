#include "ButtonFactory.h"
#include <algorithm>
#include <format>

Button ButtonFactory::Text(ButtonId id, int x, int y, const std::string& text, int width, const std::string& color)
{
    return {
        id,
        { x, y, x + width - 1, y },
        { { x, y, PadRight(text, width), color } }
    };
}

Button ButtonFactory::Box(ButtonId id, int x, int y, int width, const std::string& label, const std::string& color)
{
    int innerWidth = std::max(0, width - 2);
    std::string border = "+" + std::string(innerWidth, '-') + "+";
    std::string body = "|" + CenterInBox(label, innerWidth) + "|";

    return {
        id,
        { x, y, x + width - 1, y + 2 },
        {
            { x, y, border, color },
            { x, y + 1, body, color },
            { x, y + 2, border, color }
        }
    };
}

Button ButtonFactory::MultiLine(ButtonId id, int x, int y, int width, int height, const std::vector<ButtonLine>& lines)
{
    return {
        id,
        { x, y, x + width - 1, y + height - 1 },
        lines
    };
}

std::string ButtonFactory::PadRight(const std::string& text, int width)
{
    if (width <= 0) return text;
    if (static_cast<int>(text.size()) >= width) return text;
    return std::format("{:<{}}", text, width);
}

std::string ButtonFactory::CenterInBox(const std::string& text, int innerWidth)
{
    if (innerWidth <= 0) return "";
    int textSize = static_cast<int>(text.size());
    if (textSize >= innerWidth) return text;

    int leftPadding = (innerWidth - textSize) / 2;
    int rightPadding = innerWidth - textSize - leftPadding;
    return std::string(leftPadding, ' ') + text + std::string(rightPadding, ' ');
}
