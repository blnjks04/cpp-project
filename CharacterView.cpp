#include "CharacterView.h"
#include "ConsoleFramework.h"
CharacterView::CharacterView()
{
    playerView ={
"   [T]",
" / [_]\\",
"   / \\"
};
    
    playerSword = "--o == = >";

}

CharacterView::~CharacterView() {}

void CharacterView::DisplayPlayer(int x, int y)
{
    for (size_t i = 0; i < playerView.size(); i++)
    {
	    PRINT_TEXT(x, y + static_cast<int>(i), playerView[i]);
    }
}

void CharacterView::DisplaySword(int x, int y)
{
    PRINT_TEXT(x + static_cast<int>(playerView[1].size()), y + 1, playerSword);
}

void CharacterView::DisplayEnemy(int x, int y, const std::vector<std::string>& sprite)
{
    for (size_t i = 0; i < sprite.size(); i++)
    {
        PRINT_TEXT(x, y + static_cast<int>(i), sprite[i]);
    }
	
}
