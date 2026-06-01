#pragma once
#include "Utils.h"
#include "Node.h"

class Map // 맵
{
private:
	int currentFloor;
	int MaxFloors;
	NodeType currentNodeType;
    Node* currentNode;
    Node* nextNode;

	std::vector<std::vector<Node*>> map;
	std::vector<std::pair<NodeType, std::vector<int>>> nodeProbabilities;
public:
	Map();
	~Map();
	void GenerateMap(int floors = 16, int nodesPerFloor = 5);
	Node* GetRandomNode(Node* previousNode);
	NodeType GetRandomNodeType(std::pair<NodeType, std::vector<int>> probabilities);
	std::vector<std::vector<Node*>>& GetMap();
	int GetMaxFloors() const { return MaxFloors; }
	void ChangeNodeType(Node* node, NodeType type);
    void ChangeNodeColor(Node* node, std::string newColor);
    void SetCurrentFloor(int newFloor);
    int GetCurrentFloor();
    void SetCurrentNode(Node* node);
    Node* GetCurrentNode();
    void SetNextNode(Node* node);
    Node* GetNextNode();
};
