#include "Map.h"
#include "Logger.h"
#include <random>

Map::Map()
{
	currentFloor = 0;
	MaxFloors = 16;
	currentNodeType = NodeType::START;
	

	nodeProbabilities = {
		{NodeType::MONSTER, {4, 7, 9, 10}},    
		{NodeType::ELITE, {4, 6, 8, 10}},        
		{NodeType::EVENT, {5, 7, 9, 10}},          
		{NodeType::REST, {6, 9, 10, 11}}         
	};
    GenerateMap();
    ChangeNodeColor(map[0][0], "red");
    SetCurrentNode(map[0][0]);
    SetNextNode(map[0][0]);
}

Map::~Map() 
{
    for (auto& floor : map)
    {
        for (auto& node : floor)
        {
            delete node;
        }
    }
};

void Map::GenerateMap(int floors, int nodesPerFloor) 
{
	
    int maxNodeCount = nodesPerFloor;
	for (int i = 0; i <= floors; i++)
	{		
		std::vector<Node*> floorNodes; 
		
        if (i == 0) floorNodes.push_back(new Node(NodeType::START, {0, 1})); 
		else if (i == 1)
		{
			 for (int j = 0; j < 2; j++)
				 floorNodes.push_back(new Node(NodeType::MONSTER)); 
		}
		else if (i == floors - 1)
		{
			for (int j = 0; j < 2; j++)
				floorNodes.push_back(new Node(NodeType::REST, {0}));
		}
		else if (i == floors) floorNodes.push_back(new Node(NodeType::BOSS));
		else
		{
            int minNodeCount = 2;
            if (i == 2) minNodeCount = 3;
            std::random_device randomDevice;
            std::mt19937 randomGenerator(randomDevice());
            std::uniform_int_distribution<int> nodeCountDistribution(minNodeCount, maxNodeCount);
			for (int j = 0; j < nodeCountDistribution(randomGenerator); j++)
			{
                int previousNodeIndex = j;
                int previousLastIndex = static_cast<int>(map[i - 1].size()) - 1;
                if (previousNodeIndex >= previousLastIndex) previousNodeIndex = previousLastIndex;
                Node* node = GetRandomNode(map[i - 1][previousNodeIndex]);
                floorNodes.push_back(node);
			}

		}
		// ADD_LOG(LogType::ALL, std::format("Created floorNodes {}", i));
		map.push_back(floorNodes);
	}
    for (int i = 1; i < map.size() - 2; i++)
    {
        for (int j = 0; j < map[i].size(); j++)
        {
            if (map[i].size() == map[i + 1].size())
            {
                map[i][j]->Nodes.push_back(j);
            }
            else if (map[i].size() < map[i + 1].size())
            {
                int diff = static_cast<int>(map[i].size()) - static_cast<int>(map[i + 1].size());
                diff = abs(diff);

                int nextNodeIndex; 
                switch (diff)
                {
                case 1:
                    map[i][j]->Nodes.push_back(j);
                    map[i][j]->Nodes.push_back(j + 1);
                    break;
                case 2:
                    if (map[i].size() == 2)
                    {
                        nextNodeIndex = 0 + (j * 2);
                        map[i][j]->Nodes.push_back(nextNodeIndex);
                        map[i][j]->Nodes.push_back(nextNodeIndex + 1);
                    }
                    else if (map[i].size() == 3)
                    {                 
                        map[i][j]->Nodes.push_back(j);
                        map[i][j]->Nodes.push_back(j + 1);
                        map[i][j]->Nodes.push_back(j + 2);
                    }
                    break;
                case 3:
                    nextNodeIndex = 0 + (j * 2);
                    map[i][j]->Nodes.push_back(nextNodeIndex);
                    map[i][j]->Nodes.push_back(nextNodeIndex + 1);
                    map[i][j]->Nodes.push_back(nextNodeIndex + 2);
                    break;
                default:
                    break;
                }
                
            }
            else
            {
                int diff = static_cast<int>(map[i].size()) - static_cast<int>(map[i + 1].size());
                diff = abs(diff);
                int nextFloorNodeCount = static_cast<int>(map[i + 1].size());
                switch (diff)
                {
                case 1:
                    if (j == 0) map[i][j]->Nodes.push_back(j);
                    else if (j == nextFloorNodeCount) map[i][j]->Nodes.push_back(j - 1);
                    else
                    {
                        map[i][j]->Nodes.push_back(j - 1);
                        map[i][j]->Nodes.push_back(j);
                    }
                    break;
                case 2:              
                    if ((map[i + 1].size() == 3) && j > 2)
                    {
                        map[i][j]->Nodes.push_back(2);
                    }
                    else
                        map[i][j]->Nodes.push_back(j / 2);
                    break;
                case 3:
                    map[i][j]->Nodes.push_back(j / 3);
                    if (j == 2) map[i][j]->Nodes.push_back(1);
                    break;
                }
            }
        }
    }

	for (int i = 0; i < map.size() - 2; i++)
	{
		for (int j = 0; j < map[i].size(); j++)
		{
			for (auto connectedNodeIndex : map[i][j]->Nodes)
			{
				if (map[i][j]->nodeType == NodeType::REST && map[i][j]->nodeType == map[i + 1][connectedNodeIndex]->nodeType)
				{
					ChangeNodeType(map[i][j], GetRandomNodeType(nodeProbabilities[3]));
					break;
				}
			}
		}
	}
	ADD_LOG(LogType::StateChange, "Map Generated!");
}

Node* Map::GetRandomNode(Node* previousNode)
{
	
	switch (previousNode->nodeType)
	{
	case NodeType::MONSTER:
		return new Node(GetRandomNodeType(nodeProbabilities[0]));
	case NodeType::ELITE:
		return new Node(GetRandomNodeType(nodeProbabilities[1]));
	case NodeType::EVENT:
		return new Node(GetRandomNodeType(nodeProbabilities[2]));
	case NodeType::REST:
		return new Node(GetRandomNodeType(nodeProbabilities[3]));
	default:
		break;
	}
	return new Node(NodeType::MONSTER);
}

NodeType Map::GetRandomNodeType(const std::pair<NodeType, std::vector<int>> probabilities)
{
	std::random_device randomDevice;
	std::mt19937 randomGenerator(randomDevice());
	std::uniform_int_distribution<int> nodeTypeDistribution(1, 10);
	
	int randomValue = nodeTypeDistribution(randomGenerator);
	if (randomValue <= probabilities.second[0]) return NodeType::MONSTER;                
	else if (randomValue <= probabilities.second[1]) return NodeType::ELITE;           
	else if (randomValue <= probabilities.second[2]) return NodeType::EVENT;             
	else if (randomValue <= 10) return NodeType::REST;                                   
	return NodeType::MONSTER; 
}

void Map::ChangeNodeType(Node* node, NodeType type)
{
	ADD_LOG(LogType::StateChange, std::format("Node type [{}] changed to: [{}]", static_cast<int>(node->nodeType), static_cast<int>(type)));
	node->nodeType = type;
}

std::vector<std::vector<Node*>>& Map::GetMap()
{
	return map;
}

void Map::SetCurrentNode(Node* node)
{
    currentNode = node;
}

Node* Map::GetCurrentNode()
{
    return currentNode;
}

void Map::SetNextNode(Node* node)
{
    nextNode = node;
}

Node* Map::GetNextNode()
{
    return nextNode;
}

void Map::ChangeNodeColor(Node* node, std::string newColor)
{
    node->color = newColor;
}

void Map::SetCurrentFloor(int newFloor)
{
    currentFloor = newFloor;
}

int Map::GetCurrentFloor()
{
    return currentFloor;
}
