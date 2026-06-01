#pragma once
#include "Utils.h"

enum class CardSymbol
{
	HEART, 
	DIAMOND, 
	CLUB,    
	SPADE 
};

enum class PokerHandRank
{
	HIGH_CARD,
	ONE_PAIR,
	TWO_PAIR,
	THREE_OF_A_KIND,
	STRAIGHT,
	FLUSH,
	FULL_HOUSE,
	FOUR_OF_A_KIND,
	STRAIGHT_FLUSH,
	ROYAL_STRAIGHT_FLUSH
};

struct PokerHandResult
{
	PokerHandRank rank;
    int bestRank;
	std::string name;
	int bonusDamage;
	int bonusBlock;
	int actionPoint;
};

struct Card
{
    CardSymbol symbol;
    int rank;
    bool operator<(const Card& other) const{
        return rank < other.rank;
    }
    bool operator!=(const Card& other) const{
        if (rank != other.rank) return true;
        if (symbol != other.symbol) return true;
        return false;
    }
};
class PokerCard 
{
public:
	PokerCard();
	~PokerCard();
	std::vector<Card> CardDeck;
	void ResetDeck();
    void Shuffle();
	std::vector<Card> DrawCards(int count);
    Card DrawCard();
	PokerHandResult EvaluateHand(const std::vector<Card>& hand) const;
    PokerHandResult EvaluateBestHand(const std::vector<Card>& cards) const;
    int CompareHands(const std::vector<Card>& leftCards, const std::vector<Card>& rightCards) const;
	std::string GetCardName(const Card& card) const;
    void DisplayCard(int x, int y, const Card& card);
    void DisplayHiddenCard(int x, int y);
    std::string IntToString(int value) const;
private:
    int GetPokerTieScore(const std::vector<Card>& cards, PokerHandRank handRank) const;
    int GetBestPokerTieScore(const std::vector<Card>& cards, PokerHandRank handRank) const;
};



