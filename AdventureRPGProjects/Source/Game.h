#pragma once
#include "Utils.h"
#include "Instance.h"
#include "ConsoleFramework.h"
#include "CombatManager.h"
#include "EnemyManager.h"
#include "EventStageManager.h"
#include "UIManager.h"

class Player;

class Game : public Instance<Game>
{
	friend class Instance<Game>;
private:
    UIManager uiManager;
    EventStageManager eventStageManager;
    EnemyManager enemyManager;
    CombatManager combatManager;
public:
	Game() = default;
	~Game();

    std::unique_ptr<Player> player;
	// char currentKey;
    MouseClick click = { 0, 0 };
	void Init();
	void Run();
	void Update();
	void Render(const std::string& text, int x = 0, int y = 0);
	void Cleanup();
};
