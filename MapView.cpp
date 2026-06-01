#include "MapView.h"
#include "ConsoleFramework.h"
#include "Game.h"
#include "Map.h"
#include "Player.h"

MapView::MapView() 
{
    nodeSymbols[NodeType::START] = "[S]";
    nodeSymbols[NodeType::MONSTER] = "[M]";
    nodeSymbols[NodeType::ELITE] = "[E]";
    nodeSymbols[NodeType::EVENT] = "[?]";
    nodeSymbols[NodeType::REST] = "[#]";
    nodeSymbols[NodeType::BOSS] = "[B]";
    nodeSymbols[NodeType::PLAYER] = "[P]";
    nodeSymbols[NodeType::CLEAR] = "[X]";
}
MapView::~MapView() 
{

}

void MapView::DisplayMap(Map* map)
{
	int x = 3;
	int y = 9;
	std::string mapLine = "________________________________________________________________________________________________________________________";
	PRINT_TEXT(0, 2, mapLine);
    std::vector<std::vector<Node*>>& nodeMap = map->GetMap();

	for (auto& floor : nodeMap)
	{
		int nodeIndex = 0;
        size_t nodeCount = floor.size();
		for (auto& node : floor)
		{
            switch (nodeCount)
            {
            case 1: 
                y = 11;
                break;
            case 2:
                y = 7 + (nodeIndex * 8);
                break;
            case 3:
                y = 7 + (nodeIndex * 4);
                break;
            case 4:
                y = 5 + (nodeIndex * 4);
                break;
            case 5:
                y = 3 + (nodeIndex * 4);
                break;
            default:
                return;
                break;
            }
            node->coord.x = x;
            node->coord.y = y;

            if (nodeSymbols.find(node->nodeType) != nodeSymbols.end())
            {
                std::string& symbol = nodeSymbols[node->nodeType];
                PRINT_TEXT(node->coord.x, node->coord.y, symbol, node->color);
            }
			nodeIndex++;
		}
		x += 7;
        
	}

	PRINT_TEXT(0, 20, mapLine);
	DrawUI();
    PRINT_TEXT(60, 1, std::format("Floor : {}", map->GetCurrentFloor()), "cyan");
}

void MapView::DisplayMovableNode(Node* node)
{
    if (nodeSymbols.find(node->nodeType) != nodeSymbols.end())
    {
        std::string& symbol = nodeSymbols[node->nodeType];
        PRINT_TEXT(node->coord.x, node->coord.y, symbol, "green");
    }
}

void MapView::DrawUI()
{
	Player* player = Game::GetInstance()->player.get();
	DrawPlayerName(player);
	DrawHp(player);
	DrawLevel(player);
	DrawGold(player);
    DrawSymbolInfo();
    DrawAbliltyBox();
    DrawItemsBox();
}

void MapView::DrawHp(Player* player)
{
    if (player != nullptr)
    {
        int hp = player->Status->GetCurrentHp();
        int maxHp = player->Status->GetMaxHp();

        PRINT_TEXT(31, 1, std::format("HP : {}/{}", hp, maxHp), "red");
    }
    
}

void MapView::DrawGold(Player* player)
{
	if (player != nullptr)
	{
		int gold = player->GetGold();
		PRINT_TEXT(46, 1, std::format("Gold : {}", gold), "yellow");
	}
}

void MapView::DrawPlayerName(Player* player)
{
	if (player != nullptr)
	{
		std::string name = player->GetName();
		PRINT_TEXT(0, 1, std::format("Player({})", name));
	}
}

void MapView::DrawLevel(Player* player)
{
	if (player != nullptr)
	{
		int level = player->GetLevel();
		PRINT_TEXT(18, 1, std::format("Level : {}", level));
	}
}

void MapView::DrawSymbolInfo()
{
    PRINT_TEXT(1, 21, "[S] : 시작 지점");
    PRINT_TEXT(1, 22, "[M] : 몬스터");
    PRINT_TEXT(1, 23, "[E] : 엘리트 몬스터");
    PRINT_TEXT(1, 24, "[#] : 휴식");
    PRINT_TEXT(1, 25, "[?] : 미지");
    PRINT_TEXT(1, 26, "[B] : 보스");
    PRINT_TEXT(1, 27, "이동 가능한 곳은 초록색으로 표시 됩니다.");
    PRINT_TEXT(1, 28, "클리어 한 곳은 빨간색으로 표시 됩니다.");
    PRINT_TEXT(1, 29, "초록색으로 표시된 곳을 클릭하면 해당 맵으로 이동합니다.");
}

void MapView::DrawAbliltyBox()
{
    PRINT_TEXT(70, 22, " __________ ");
    PRINT_TEXT(70, 23, "|          |");
    PRINT_TEXT(70, 24, "| 플레이어 |");
    PRINT_TEXT(70, 25, "|   정보   |");
    PRINT_TEXT(70, 26, "|          |");
    PRINT_TEXT(70, 27, " ---------- ");
}

void MapView::DrawItemsBox()
{
    PRINT_TEXT(90, 22, " __________ ");
    PRINT_TEXT(90, 23, "|          |");
    PRINT_TEXT(90, 24, "|   인벤   |");
    PRINT_TEXT(90, 25, "|   토리   |");
    PRINT_TEXT(90, 26, "|          |");
    PRINT_TEXT(90, 27, " ---------- ");
}
