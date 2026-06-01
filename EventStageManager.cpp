#include "EventStageManager.h"
#include "GameDataManager.h"
#include "GameStateManager.h"
#include "PokerCard.h"
#include "Logger.h"

#include "Player.h"
#include "ButtonRenderer.h"
#include "EventButtons.h"

void EventStageManager::StartEvent(Player* player)
{
    if (player == nullptr)
    {
        CHANGE_STATE(GameState::MAP);
        return;
    }

    if (GameDataManager::GetInstance()->GetEventStages().empty() &&
        !GameDataManager::GetInstance()->LoadEventStageData("DB_EventStage.json"))
    {
        CHANGE_STATE(GameState::MAP);
        return;
    }

    currentEvent = GetRandomEvent();
    CLEAR_SCREEN();
    DisplayEvent(player, currentEvent);

    while (true)
    {
        MouseClick click = GET_MOUSE_CLICK();
        int choiceIndex = GetClickedChoiceIndex(click, currentEvent);
        if (choiceIndex < 0) continue;

        const EventChoice& choice = currentEvent.choices[choiceIndex];
        if (choice.bIsPokerBattle)
        {
            PokerBattleEvent(player);
            CHANGE_STATE(choice.nextState);
            break;
        }

        ApplyChoice(player, choice);
        DisplayResult(player, choice);
        GET_MOUSE_CLICK();
        CHANGE_STATE(choice.nextState);
        break;
    }
}

EventStage EventStageManager::GetRandomEvent()
{
    const std::vector<EventStage>& eventStages = GameDataManager::GetInstance()->GetEventStages();

    std::random_device randomDevice;
    std::mt19937 randomGenerator(randomDevice());
    std::uniform_int_distribution<int> eventIndexDistribution(0, static_cast<int>(eventStages.size()) - 1);
    return eventStages[eventIndexDistribution(randomGenerator)];
}

void EventStageManager::DisplayEvent(Player* player, const EventStage& eventStage)
{
    PRINT_TEXT(0, 0, "========================================================================================================================");
    PRINT_TEXT(2, 1, "UNKNOWN EVENT");
    PRINT_TEXT(82, 1, std::format("HP {} / {}", player->Status->GetCurrentHp(), player->Status->GetMaxHp()), "red");
    PRINT_TEXT(0, 2, "========================================================================================================================");

    PRINT_TEXT(8, 5, std::format("[ {} ]", eventStage.stageName), "yellow");

    for (int i = 0; i < eventStage.description.size(); i++)
    {
        PRINT_TEXT(8, 7 + i, eventStage.description[i]);
    }

    int choiceStartY = GetChoiceStartY(eventStage);
    PRINT_TEXT(8, choiceStartY - 2, "선택지를 클릭하세요.");

    for (int i = 0; i < eventStage.choices.size(); i++)
    {
        int y = choiceStartY + (i * 3);
        ButtonRenderer::Draw(EventButtons::Choice(i, y, eventStage.choices[i].text));
    }

    PRINT_TEXT(0, 27, "========================================================================================================================");
}

int EventStageManager::GetClickedChoiceIndex(const MouseClick& click, const EventStage& eventStage)
{
    int choiceStartY = GetChoiceStartY(eventStage);
    for (int i = 0; i < eventStage.choices.size(); i++)
    {
        int y = choiceStartY + (i * 3);
        if (ButtonRenderer::IsClicked(EventButtons::Choice(i, y, eventStage.choices[i].text), click))
        {
            return i;
        }
    }
    return -1;
}

int EventStageManager::GetChoiceStartY(const EventStage& eventStage) const
{
    int descriptionLineCount = static_cast<int>(eventStage.description.size());
    return 10 + descriptionLineCount;
}

void EventStageManager::ApplyChoice(Player* player, const EventChoice& choice)
{
    if (choice.maxHpChange != 0)
    {
        int nextMaxHp = player->Status->GetMaxHp() + choice.maxHpChange;
        if (nextMaxHp < 1) nextMaxHp = 1;
        player->Status->SetMaxHp(nextMaxHp);
    }

    if (choice.hpChange != 0)
    {
        int nextHp = player->Status->GetCurrentHp() + choice.hpChange;
        if (nextHp > player->Status->GetMaxHp()) nextHp = player->Status->GetMaxHp();
        if (nextHp < 0) nextHp = 0;
        player->Status->SetCurrentHp(nextHp);
        if (nextHp == 0) player->Status->SetBIsAlive(false);
    }

    if (choice.bIsLevelUp && player->Status->IsAlive())
    {
        player->LevelUp();
    }

    ADD_LOG(LogType::StateChange, std::format("Event choice selected: {}", choice.text));
}

void EventStageManager::DisplayResult(Player* player, const EventChoice& choice)
{
    CLEAR_SCREEN();
    PRINT_TEXT(0, 0, "========================================================================================================================");
    PRINT_TEXT(2, 1, "EVENT RESULT");
    PRINT_TEXT(82, 1, std::format("HP {} / {}", player->Status->GetCurrentHp(), player->Status->GetMaxHp()), "red");
    PRINT_TEXT(0, 2, "========================================================================================================================");

    PRINT_TEXT(8, 8, choice.resultText, "yellow");
    PRINT_TEXT(8, 12, "계속하려면 아무 곳이나 클릭하세요.");
    PRINT_TEXT(0, 27, "========================================================================================================================");
}

void EventStageManager::PokerBattleEvent(Player* player)
{
    CLEAR_SCREEN();
    PokerCard pokerCard;
    pokerCard.ResetDeck();
    pokerCard.Shuffle();

    std::vector<Card> playerCards = pokerCard.DrawCards(7);
    std::vector<Card> dealerCards = pokerCard.DrawCards(7);
    int hpBet = 0;
    int attackBet = 0;
    std::string message = "카드를 받을 때마다 HP 또는 공격력을 배팅할 수 있습니다.";

    int round;
    for (round = 3; round <= 7; round++)
    {
        
        DisplayPokerBattle(player, playerCards, dealerCards, hpBet, attackBet, round);
        if (round == 7) message = "마지막 배팅입니다.";
        PRINT_TEXT(2, 26, "                                                                          ");
        std::vector<Button> betButtons = EventButtons::PokerBetButtons();
        ButtonRenderer::DrawAll(betButtons);
        PRINT_TEXT(2, 24, "                                                                         ");
        PRINT_TEXT(2, 24, message, "yellow");
        while (true)
        {
        
            MouseClick click = GET_MOUSE_CLICK();
            int clickedBetIndex = ButtonRenderer::GetClickedIndex(betButtons, click);
            ButtonId clickedBetId = clickedBetIndex >= 0 ? betButtons[clickedBetIndex].id : ButtonId::None;
            if (clickedBetId == ButtonId::PokerBetHp5)
            {
                if (player->Status->GetCurrentHp() - hpBet > 5)
                {
                    hpBet += 5;
                    message = "HP 5를 추가로 배팅했습니다.         ";
                    break;
                }
                message = "패배해도 HP 1은 남아야 합니다.";
            }
            else if (clickedBetId == ButtonId::PokerBetHp10)
            {
                if (player->Status->GetCurrentHp() - hpBet > 10)
                {
                    hpBet += 10;
                    message = "HP 10을 추가로 배팅했습니다.         ";
                    break;
                }
                message = "패배해도 HP 1은 남아야 합니다.";
            }
            else if (clickedBetId == ButtonId::PokerBetAttack1)
            {
                if (player->GetBaseDamage() - attackBet > 1)
                {
                    attackBet += 1;
                    message = "공격력 1을 추가로 배팅했습니다.          ";
                    break;
                }
                message = "패배해도 공격력 1은 남아야 합니다.";
            }
            else if (clickedBetId == ButtonId::PokerBetAttack2)
            {
                if (player->GetBaseDamage() - attackBet > 2)
                {
                    attackBet += 2;
                    message = "공격력 2를 추가로 배팅했습니다.           ";
                    break;
                }
                message = "패배해도 공격력 1은 남아야 합니다.";
            }
            else if (clickedBetId == ButtonId::PokerCheck)
            {
                message = "이번 카드는 배팅하지 않았습니다.               ";
                break;
            }
            PRINT_TEXT(2, 24, message, "yellow");
        }
        Button nextButton = EventButtons::PokerNext(round == 7);
        ButtonRenderer::Draw(nextButton);
        PRINT_TEXT(2, 24, "                                                                          ");
        PRINT_TEXT(2, 24, message, "yellow");
        PRINT_TEXT(2, 3, std::format("현재 배팅: HP {} / 공격력 {}", hpBet, attackBet), "yellow");
        while (!ButtonRenderer::IsClicked(nextButton, GET_MOUSE_CLICK()));
        message = "HP 또는 공격력을 배팅할 수 있습니다.";
    }
    

    int result = pokerCard.CompareHands(playerCards, dealerCards);
    PokerHandResult playerResult = pokerCard.EvaluateBestHand(playerCards);
    PokerHandResult dealerResult = pokerCard.EvaluateBestHand(dealerCards);

    if (result > 0)
    {
        int nextHp = player->Status->GetCurrentHp() + (hpBet * 2);
        if (nextHp > player->Status->GetMaxHp()) nextHp = player->Status->GetMaxHp();
        player->Status->SetCurrentHp(nextHp);
        player->AddBaseDamage(attackBet * 2);
        message = std::format("승리! HP +{}, 공격력 +{} 보상을 얻었습니다.", hpBet * 2, attackBet * 2);
    }
    else if (result < 0)
    {
        int nextHp = player->Status->GetCurrentHp() - hpBet;
        if (nextHp < 0) nextHp = 0;
        player->Status->SetCurrentHp(nextHp);
        if (nextHp == 0) player->Status->SetBIsAlive(false);
        player->AddBaseDamage(-attackBet);
        message = std::format("패배... HP -{}, 공격력 -{} 손실을 입었습니다.", hpBet, attackBet);
    }
    else
    {
        message = "무승부입니다. 배팅한 자원은 변하지 않습니다.";
    }

    CLEAR_SCREEN();
    DisplayPokerBattle(player, playerCards, dealerCards, hpBet, attackBet, 8);
    PRINT_TEXT(2, 23, std::format("플레이어 족보: {} [{}]    딜러 족보: {} [{}]", playerResult.name, pokerCard.IntToString(playerResult.bestRank), dealerResult.name, pokerCard.IntToString(dealerResult.bestRank)), "yellow");
    PRINT_TEXT(2, 25, message, result >= 0 ? "yellow" : "red");
    PRINT_TEXT(2, 27, "계속하려면 아무 곳이나 클릭하세요.");
    GET_MOUSE_CLICK();
}

void EventStageManager::DisplayPokerBattle(Player* player, const std::vector<Card>& playerCards, const std::vector<Card>& dealerCards, int hpBet, int attackBet, int round)
{
    PokerCard pokerCard;
    PRINT_TEXT(0, 0, "========================================================================================================================");
    PRINT_TEXT(2, 1, "UNKNOWN EVENT - 의문의 딜러");
    PRINT_TEXT(62, 1, std::format("HP {} / {}   ATK {}", player->Status->GetCurrentHp(), player->Status->GetMaxHp(), player->GetBaseDamage()), "red");
    PRINT_TEXT(0, 2, "========================================================================================================================");
    PRINT_TEXT(2, 3, std::format("현재 배팅: HP {} / 공격력 {}", hpBet, attackBet), "yellow");
    PRINT_TEXT(2, 5, "Dealer");
    PRINT_TEXT(2, 16, "Player");

    for (int i = 0; i < 7; i++)
    {
        int x = 12 + (i * 10);
        if (i < round)
        {
            if ((i == 0 || i == 1 || i == 6) && round < 8) pokerCard.DisplayHiddenCard(x, 5);
            else pokerCard.DisplayCard(x, 5, dealerCards[i]);
            pokerCard.DisplayCard(x, 16, playerCards[i]);
        }
        else
        {
            pokerCard.DisplayHiddenCard(x, 5);
            pokerCard.DisplayHiddenCard(x, 16);
        }
    }

    PRINT_TEXT(0, 14, "========================================================================================================================");
    PRINT_TEXT(0, 29, "========================================================================================================================");
}
