#pragma once
#include "View.h"

class Player;

class PlayerInfoView : public View
{
public:
    PlayerInfoView() = default;
    ~PlayerInfoView() = default;

    void DisplayPlayerInfo(Player* player);
};

