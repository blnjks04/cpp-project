#include "ButtonRenderer.h"

void ButtonRenderer::Draw(const Button& button)
{
    for (const ButtonLine& line : button.lines)
    {
        PRINT_TEXT(line.x, line.y, line.text, line.color);
    }
}

void ButtonRenderer::DrawAll(const std::vector<Button>& buttons)
{
    for (const Button& button : buttons)
    {
        Draw(button);
    }
}

bool ButtonRenderer::IsClicked(const Button& button, const MouseClick& click)
{
    return IS_IN_AREA(click, button.rect.left, button.rect.top, button.rect.right, button.rect.bottom);
}

int ButtonRenderer::GetClickedIndex(const std::vector<Button>& buttons, const MouseClick& click)
{
    for (int i = 0; i < static_cast<int>(buttons.size()); i++)
    {
        if (IsClicked(buttons[i], click)) return i;
    }
    return -1;
}
