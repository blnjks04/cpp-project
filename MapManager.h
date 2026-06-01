#pragma once
#include "Utils.h"
#include "Instance.h"
#include "ConsoleFramework.h"

class Map;
class MapView;

class MapManager : public Instance<MapManager>
{
	friend class Instance<MapManager>;
private:
	MapManager();
	~MapManager() = default;
	std::unique_ptr<Map> map;
    std::unique_ptr<MapView> mapView;
    int currentFloor;
public:
	void MovePlayer();
    bool IsClickedNextNode(const MouseClick& click);
    bool IsClickedPlayerInfoBox(const MouseClick& click);
    void DisplayMap();
    void SetColorMovableNode();
    int GetCurrentFloor();
};
