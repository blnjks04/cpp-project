#pragma once
#include "Utils.h"

enum class NodeType
{
	START,
	PLAYER,
	MONSTER,
	ELITE,
	REST,
	EVENT,
	BOSS,
	CLEAR
};

struct Coord
{
    int x;
    int y;
};
struct Node
{
	NodeType nodeType;
	std::vector<int> Nodes;
    Coord coord;
    std::string color;
	Node(NodeType type, const std::vector<int>& nodes = {}, Coord coord = {0, 0}, std::string color = "white") : nodeType(type), Nodes(nodes), coord(coord), color(color) {}
};





