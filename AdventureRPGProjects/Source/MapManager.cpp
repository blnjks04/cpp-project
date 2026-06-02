#include "MapManager.h"
#include "Logger.h"
#include "GameStateManager.h"
#include "Map.h"
#include "MapView.h"

MapManager::MapManager()
{
	map = std::make_unique<Map>();
    mapView = std::make_unique<MapView>();
    currentFloor = 0;
    SetColorMovableNode();
}


void MapManager::MovePlayer()
{
    map->SetCurrentFloor(++currentFloor);

    std::vector<std::vector<Node*>>& nodeMap = map->GetMap();
    for (auto& node : nodeMap[currentFloor])
    {
        node->color = "white";
    }

    
    Node* currentNode = map->GetCurrentNode();
    Node* nextNode = map->GetNextNode();
    
    ADD_LOG(LogType::ALL, "moved player");
    

    map->SetCurrentNode(nextNode);
    map->ChangeNodeColor(nextNode, "red");
    switch (nextNode->nodeType)
    {
    case NodeType::MONSTER:
        CHANGE_STATE(GameState::BATTLE);
        break;
    case NodeType::ELITE:
        CHANGE_STATE(GameState::ELITE_BATTLE);
        break;
    case NodeType::EVENT:
        CHANGE_STATE(GameState::EVENT);
        break;
    case NodeType::REST:
        CHANGE_STATE(GameState::REST);
        break;
    case NodeType::BOSS:
        CHANGE_STATE(GameState::BOSS_BATTLE);
        break;
    }
    SetColorMovableNode();
}

bool MapManager::IsClickedNextNode(const MouseClick& click)
{
    std::vector<std::vector<Node*>>& nodeMap = map->GetMap();
    Node* currentNode = map->GetCurrentNode();
    for (auto& nextNodeIndex : currentNode->Nodes)
    {
        int left = nodeMap[currentFloor + 1][nextNodeIndex]->coord.x;
        int right = left + 2;
        int top = nodeMap[currentFloor + 1][nextNodeIndex]->coord.y;
        int bottom = top + 1;
        if (IS_IN_AREA(click, left, top, right, bottom))
        {
            map->SetNextNode(nodeMap[currentFloor + 1][nextNodeIndex]);
            return true;
        }
    }
    return false;
}

bool MapManager::IsClickedPlayerInfoBox(const MouseClick& click)
{
    return IS_IN_AREA(click, 70, 22, 81, 27);
}

void MapManager::SetColorMovableNode()
{
    std::vector<std::vector<Node*>>& nodeMap = map->GetMap();
    for (auto& nextNodeIndex : map->GetCurrentNode()->Nodes)
    {
        nodeMap[currentFloor + 1][nextNodeIndex]->color = "green";
    }
}

void MapManager::DisplayMap()
{
    mapView->DisplayMap(map.get());
}

int MapManager::GetCurrentFloor()
{
    return currentFloor;
}
