#pragma once
#include "Utils.h"
#include "Node.h"
#include "View.h"

class Map;
class Player;

class MapView : public View
{
private:
    std::map<NodeType, std::string> nodeSymbols;

public:
    MapView();
    ~MapView();

    void DisplayMap(Map* map);
    void DisplayMovableNode(Node* node);
    void DrawUI();
    void DrawHp(Player* player);
    void DrawLevel(Player* player);
    void DrawGold(Player* player);
    void DrawPlayerName(Player* player);
    void DrawSymbolInfo();
    void DrawAbliltyBox();
    void DrawItemsBox();
};
