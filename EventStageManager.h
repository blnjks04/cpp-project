#pragma once
#include "Utils.h"
#include "ConsoleFramework.h"
#include "EventStage.h"
#include "PokerCard.h"

class Player;

class EventStageManager
{
private:
    EventStage currentEvent;

    EventStage GetRandomEvent();
    void DisplayEvent(Player* player, const EventStage& eventStage);
    int GetClickedChoiceIndex(const MouseClick& click, const EventStage& eventStage);
    int GetChoiceStartY(const EventStage& eventStage) const;
    void ApplyChoice(Player* player, const EventChoice& choice);
    void DisplayResult(Player* player, const EventChoice& choice);
    void DisplayPokerBattle(Player* player, const std::vector<Card>& playerCards, const std::vector<Card>& dealerCards, int hpBet, int attackBet, int round);
public:
    EventStageManager() {};
    ~EventStageManager() {};

    void StartEvent(Player* player);
    void PokerBattleEvent(Player* player);
};
