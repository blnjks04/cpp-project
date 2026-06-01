#include "PokerCard.h"
#include "ConsoleFramework.h"

PokerCard::PokerCard() 
{
    ResetDeck();
}

PokerCard::~PokerCard() 
{
}

void PokerCard::ResetDeck()
{
    CardDeck.clear();
	for (int i = 2; i <= 14; i++)
	{
		
		CardDeck.push_back({CardSymbol::HEART, i});
		CardDeck.push_back({CardSymbol::DIAMOND, i});
		CardDeck.push_back({CardSymbol::CLUB, i});
		CardDeck.push_back({CardSymbol::SPADE, i});
		
	}	
}

std::vector<Card> PokerCard::DrawCards(int count)
{
    if (CardDeck.size() < count)
    {
        ResetDeck();
        Shuffle();
    }

    std::vector<Card> hand;
    for (int i = 0; i < count && !CardDeck.empty(); i++)
    {
        hand.push_back(CardDeck.back());
        CardDeck.pop_back();
    }
    return hand;
}

Card PokerCard::DrawCard()
{

    if (CardDeck.size() < 1)
    {
        ResetDeck();
        Shuffle();
    }

    Card card = CardDeck.back();
    CardDeck.pop_back();
    return card;
}
void PokerCard::DisplayCard(int x, int y, const Card& card)
{
    std::string symbol;
    std::string color;
    std::string rank = IntToString(card.rank);
	switch (card.symbol)
	{
	case CardSymbol::HEART:
		symbol = "♥ ";
        color = "red";
		break;
	case CardSymbol::DIAMOND:
		symbol = "♦️ ";
        color = "red";
		break;
	case CardSymbol::CLUB:
		symbol = "♣ ";
        color = "white";
		break;
	case CardSymbol::SPADE:
		symbol = "♠ ";
        color = "white";
		break;	
	default:
		symbol = "?";
		break;
	}
    std::vector<std::string> cardFrameLines = {
 " ------- ",
 "|       |",
 "|       |",
 "|       |",
 "|       |",
 "|       |",
 " ------- " };
    int i = 0;
    for (auto& line : cardFrameLines)
    {
        PRINT_TEXT(x, y + i++, line);
    }
    PRINT_TEXT_SLOW(x + 1, y + 1, rank, color);
    PRINT_TEXT_SLOW(x + 4, y + 3, symbol, color);
    if (card.rank == 10) x--;
    PRINT_TEXT_SLOW(x + 7, y + 5, rank, color);
}
void PokerCard::DisplayHiddenCard(int x, int y)
{
    std::vector<std::string> cardFrameLines = {
        " ------- ",
        "|///////|",
        "|///////|",
        "|///?///|",
        "|///////|",
        "|///////|",
        " ------- "
    };

    for (int i = 0; i < cardFrameLines.size(); i++)
    {
        PRINT_TEXT(x, y + i, cardFrameLines[i], "blue");
    }
}
PokerHandResult PokerCard::EvaluateHand(const std::vector<Card>& hand) const
{
    std::map<int, int> rankCounts;
    bool isFlush = !hand.empty();
    CardSymbol firstSymbol = hand.empty() ? CardSymbol::HEART : hand.front().symbol;
    std::vector<int> ranks;

    for (const auto& card : hand)
    {
        rankCounts[card.rank]++;
        ranks.push_back(card.rank);
        if (card.symbol != firstSymbol)
        {
            isFlush = false;
        }
    }

    std::sort(ranks.begin(), ranks.end());
    ranks.erase(std::unique(ranks.begin(), ranks.end()), ranks.end());

    bool isStraight = ranks.size() == 5 && ranks.back() - ranks.front() == 4;
    int straightBestRank = isStraight ? ranks.back() : 0;
    if (!isStraight && ranks == std::vector<int>{2, 3, 4, 5, 14})
    {
        isStraight = true;
        straightBestRank = 5;
    }

    bool isRoyal = ranks == std::vector<int>{10, 11, 12, 13, 14};
    int pairs = 0;
    bool hasThree = false;
    bool hasFour = false;
    int highRank = ranks.empty() ? 0 : ranks.back();
    int pairBestRank = 0;
    int threeBestRank = 0;
    int fourBestRank = 0;

    for (const auto& rankCount : rankCounts)
    {
        if (rankCount.second == 2)
        {
            pairs++;
            if (pairBestRank < rankCount.first) pairBestRank = rankCount.first;
        }
        if (rankCount.second == 3)
        {
            hasThree = true;
            if (threeBestRank < rankCount.first) threeBestRank = rankCount.first;
        }
        if (rankCount.second == 4)
        {
            hasFour = true;
            if (fourBestRank < rankCount.first) fourBestRank = rankCount.first;
        }
    }

    if (isFlush && isRoyal)
    {
        return { PokerHandRank::ROYAL_STRAIGHT_FLUSH, 14, "[ROYAL STRAIGHT FLUSH]", 999, 999, 99 };
    }
    if (isFlush && isStraight)
    {
        return { PokerHandRank::STRAIGHT_FLUSH, straightBestRank, "[STRAIGHT FLUSH]", 100, 50, 10 };
    }
    if (hasFour)
    {
        return { PokerHandRank::FOUR_OF_A_KIND, fourBestRank, "[FOUR CARD]", 10, 5, 5 };
    }
    if (hasThree && pairs == 1)
    {
        return { PokerHandRank::FULL_HOUSE, threeBestRank, "[FULL HOUSE]", 5, 3, 4 };
    }
    if (isFlush)
    {
        return { PokerHandRank::FLUSH, highRank, "[FLUSH]", 6, 3, 3 };
    }
    if (isStraight)
    {
        return { PokerHandRank::STRAIGHT, straightBestRank, "[STRAIGHT]", 4, 2, 3 };
    }
    if (hasThree)
    {
        return { PokerHandRank::THREE_OF_A_KIND, threeBestRank, "[TRIPLE]", 2, 1, 2 };
    }
    if (pairs == 2)
    {
        return { PokerHandRank::TWO_PAIR, pairBestRank, "[TWO PAIR]", 4, 2, 1 };
    }
    if (pairs == 1)
    {
        return { PokerHandRank::ONE_PAIR, pairBestRank, "[ONE PAIR]", 2, 1, 1 };
    }
    return { PokerHandRank::HIGH_CARD, highRank, "[TOP]", 0, 0, 1 };
}

PokerHandResult PokerCard::EvaluateBestHand(const std::vector<Card>& cards) const
{
    PokerHandResult bestResult = { PokerHandRank::HIGH_CARD, 0, "[TOP]", 0, 0, 1 };
    int bestTieScore = -1;

    if (cards.size() < 5)
    {
        return EvaluateHand(cards);
    }
    for (int a = 0; a < static_cast<int>(cards.size()) - 4; a++)
    {
        for (int b = a + 1; b < static_cast<int>(cards.size()) - 3; b++)
        {
            for (int c = b + 1; c < static_cast<int>(cards.size()) - 2; c++)
            {
                for (int d = c + 1; d < static_cast<int>(cards.size()) - 1; d++)
                {
                    for (int e = d + 1; e < static_cast<int>(cards.size()); e++)
                    {
                        std::vector<Card> hand = { cards[a], cards[b], cards[c], cards[d], cards[e] };
                        PokerHandResult result = EvaluateHand(hand);
                        int tieScore = GetPokerTieScore(hand, result.rank);
                        if (static_cast<int>(result.rank) > static_cast<int>(bestResult.rank) ||
                            (result.rank == bestResult.rank && tieScore > bestTieScore))
                        {
                            bestResult = result;
                            bestTieScore = tieScore;
                        }
                    }
                }
            }
        }
    }

    return bestResult;
}

int PokerCard::CompareHands(const std::vector<Card>& leftCards, const std::vector<Card>& rightCards) const
{
    PokerHandResult leftResult = EvaluateBestHand(leftCards);
    PokerHandResult rightResult = EvaluateBestHand(rightCards);

    if (static_cast<int>(leftResult.rank) > static_cast<int>(rightResult.rank)) return 1;
    if (static_cast<int>(leftResult.rank) < static_cast<int>(rightResult.rank)) return -1;
    if (leftResult.bestRank > rightResult.bestRank) return 1;
    if (leftResult.bestRank < rightResult.bestRank) return -1;

    int leftTieScore = GetBestPokerTieScore(leftCards, leftResult.rank);
    int rightTieScore = GetBestPokerTieScore(rightCards, rightResult.rank);
    if (leftTieScore > rightTieScore) return 1;
    if (leftTieScore < rightTieScore) return -1;
    return 0;
}

int PokerCard::GetPokerTieScore(const std::vector<Card>& cards, PokerHandRank handRank) const
{
    std::map<int, int> rankCounts;
    for (const auto& card : cards)
    {
        rankCounts[card.rank]++;
    }

    auto encodeRanks = [](const std::vector<int>& ranks)
        {
            int score = 0;
            for (int rank : ranks)
            {
                score = (score * 15) + rank;
            }
            return score;
        };

    auto getDescendingRanks = [&rankCounts]()
        {
            std::vector<int> ranks;
            for (const auto& rankCount : rankCounts)
            {
                ranks.push_back(rankCount.first);
            }
            std::sort(ranks.begin(), ranks.end(), [](int left, int right) { return left > right; });
            return ranks;
        };

    auto getStraightBestRank = [&rankCounts]()
        {
            std::vector<int> ranks;
            for (const auto& rankCount : rankCounts)
            {
                ranks.push_back(rankCount.first);
            }
            std::sort(ranks.begin(), ranks.end());
            ranks.erase(std::unique(ranks.begin(), ranks.end()), ranks.end());

            if (ranks == std::vector<int>{2, 3, 4, 5, 14})
            {
                return 5;
            }
            if (ranks.size() == 5 && ranks.back() - ranks.front() == 4)
            {
                return ranks.back();
            }
            return ranks.empty() ? 0 : ranks.back();
        };

    auto getRanksByCount = [&rankCounts](int count)
        {
            std::vector<int> ranks;
            for (const auto& rankCount : rankCounts)
            {
                if (rankCount.second == count)
                {
                    ranks.push_back(rankCount.first);
                }
            }
            std::sort(ranks.begin(), ranks.end(), [](int left, int right) { return left > right; });
            return ranks;
        };

    if (handRank == PokerHandRank::ROYAL_STRAIGHT_FLUSH)
    {
        return 14;
    }
    if (handRank == PokerHandRank::STRAIGHT_FLUSH || handRank == PokerHandRank::STRAIGHT)
    {
        return getStraightBestRank();
    }
    if (handRank == PokerHandRank::FLUSH || handRank == PokerHandRank::HIGH_CARD)
    {
        return encodeRanks(getDescendingRanks());
    }

    std::vector<int> fourRanks = getRanksByCount(4);
    std::vector<int> threeRanks = getRanksByCount(3);
    std::vector<int> pairRanks = getRanksByCount(2);
    std::vector<int> singleRanks = getRanksByCount(1);

    if (handRank == PokerHandRank::FOUR_OF_A_KIND && !fourRanks.empty())
    {
        std::vector<int> compareRanks = { fourRanks.front() };
        compareRanks.insert(compareRanks.end(), singleRanks.begin(), singleRanks.end());
        return encodeRanks(compareRanks);
    }

    if (handRank == PokerHandRank::FULL_HOUSE && !threeRanks.empty() && !pairRanks.empty())
    {
        return encodeRanks({ threeRanks.front(), pairRanks.front() });
    }

    if (handRank == PokerHandRank::THREE_OF_A_KIND && !threeRanks.empty())
    {
        std::vector<int> compareRanks = { threeRanks.front() };
        compareRanks.insert(compareRanks.end(), singleRanks.begin(), singleRanks.end());
        return encodeRanks(compareRanks);
    }

    if (handRank == PokerHandRank::TWO_PAIR && pairRanks.size() >= 2)
    {
        std::vector<int> compareRanks = { pairRanks[0], pairRanks[1] };
        compareRanks.insert(compareRanks.end(), singleRanks.begin(), singleRanks.end());
        return encodeRanks(compareRanks);
    }

    if (handRank == PokerHandRank::ONE_PAIR && !pairRanks.empty())
    {
        std::vector<int> compareRanks = { pairRanks.front() };
        compareRanks.insert(compareRanks.end(), singleRanks.begin(), singleRanks.end());
        return encodeRanks(compareRanks);
    }

    std::vector<std::pair<int, int>> countRanks;
    for (const auto& rankCount : rankCounts)
    {
        countRanks.push_back({ rankCount.second, rankCount.first });
    }

    std::sort(countRanks.begin(), countRanks.end(), [](const auto& left, const auto& right)
        {
            if (left.first == right.first) return left.second > right.second;
            return left.first > right.first;
        });

    int score = 0;
    for (const auto& countRank : countRanks)
    {
        score = (score * 100) + (countRank.first * 15) + countRank.second;
    }
    return score;
}

int PokerCard::GetBestPokerTieScore(const std::vector<Card>& cards, PokerHandRank handRank) const
{
    int bestTieScore = -1;

    if (cards.size() < 5)
    {
        return GetPokerTieScore(cards, EvaluateHand(cards).rank);
    }

    for (int a = 0; a < static_cast<int>(cards.size()) - 4; a++)
    {
        for (int b = a + 1; b < static_cast<int>(cards.size()) - 3; b++)
        {
            for (int c = b + 1; c < static_cast<int>(cards.size()) - 2; c++)
            {
                for (int d = c + 1; d < static_cast<int>(cards.size()) - 1; d++)
                {
                    for (int e = d + 1; e < static_cast<int>(cards.size()); e++)
                    {
                        std::vector<Card> hand = { cards[a], cards[b], cards[c], cards[d], cards[e] };
                        PokerHandResult result = EvaluateHand(hand);
                        if (result.rank == handRank)
                        {
                            int tieScore = GetPokerTieScore(hand, result.rank);
                            if (tieScore > bestTieScore) bestTieScore = tieScore;
                        }
                    }
                }
            }
        }
    }

    return bestTieScore;
}

std::string PokerCard::GetCardName(const Card& card) const
{
    std::string symbol;
    switch (card.symbol)
    {
    case CardSymbol::HEART:
        symbol = "Heart";
        break;
    case CardSymbol::DIAMOND:
        symbol = "Diamond";
        break;
    case CardSymbol::CLUB:
        symbol = "Club";
        break;
    case CardSymbol::SPADE:
        symbol = "Spade";
        break;
    default:
        symbol = "?";
        break;
    }

    std::string rank = IntToString(card.rank);
   

    return std::format("{} {}", symbol, rank);
}

void PokerCard::Shuffle()
{
    std::random_device randomDevice;
    std::mt19937 randomGenerator(randomDevice());
    
    std::shuffle(CardDeck.begin(), CardDeck.end(), randomGenerator);
}

std::string PokerCard::IntToString(int value) const
{
    switch (value)
    {
    case 11:
        return "J";
    case 12:
        return "Q";
    case 13:
        return "K";
    case 14:
        return "A";
    default:
        return std::to_string(value);
    }
}
