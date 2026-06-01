#pragma once
#include "ButtonRenderer.h"

class View
{
protected:
    void Clear() const { CLEAR_SCREEN(); }
    void DrawText(int x, int y, const std::string& text, const std::string& color = "white") const { PRINT_TEXT(x, y, text, color); }
    void DrawButton(const Button& button) const { ButtonRenderer::Draw(button); }
    bool IsClickedButton(const Button& button, const MouseClick& click) const { return ButtonRenderer::IsClicked(button, click); }
};
