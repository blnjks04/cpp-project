#pragma once
#include "Button.h"

class CombatButtons
{
public:
    static Button DrawCards();
    static Button ConfirmHand();
    static Button FixedCard(int index, bool bIsFixed = false);
    static Button Attack(int damage = 0);
    static Button Block(int block = 0);
    static Button Reroll();
    static Button TurnEnd();
    static Button Escape();
    static Button EscapeConfirmYes();
};
