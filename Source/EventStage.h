#pragma once
#include "Utils.h"
#include "GameStateManager.h"

// 이벤트 선택지
struct EventChoice
{
    std::string text;
    int hpChange;
    int maxHpChange;
    bool bIsLevelUp;
    bool bIsBattle;
    bool bIsPokerBattle;
    std::string resultText;
    GameState nextState;
};

struct EventStage
{
    std::string stageName;
    std::vector<std::string> description;
    std::vector<EventChoice> choices;
};
