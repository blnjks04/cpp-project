#pragma once
#include "Utils.h"
#include "GameStateManager.h"
#include "PokerCard.h"

class Player;
class Enemy;
class CombatRuleEffect;

struct CombatContext
{
    Player* player = nullptr;
    std::vector<Enemy*> enemies;
    GameState battleState = GameState::BATTLE;
    bool bIsBattle = true;
    bool bIsVictory = true;
    std::string battleResult = "승리";

    PokerCard pokerCard;
    std::vector<Card> hand;
    PokerHandResult handResult = { PokerHandRank::HIGH_CARD, 0, "[TOP]", 0, 0, 1 };
    std::vector<bool> fixedCards;
    std::string message;

    std::vector<CombatRuleEffect*> ruleEffects;
};
