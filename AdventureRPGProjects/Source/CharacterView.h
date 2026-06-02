#pragma once
#include "Utils.h"
#include "View.h"

class CharacterView : public View
{
public:
    std::vector<std::string> playerView;
    std::string playerSword;
    CharacterView();
    virtual ~CharacterView();
    void DisplayPlayer(int x = 0, int y = 0);
    void DisplaySword(int x, int y);
    void DisplayEnemy(int x, int y, const std::vector<std::string>& sprite);
    
};
