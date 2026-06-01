#pragma once
#include "Utils.h"
#include "Instance.h"
enum class GameState
{
	NONE,
	MENU,
	MAP,
	EVENT,
    REST,
	RESULT,
	BATTLE,
	ELITE_BATTLE,
	BOSS_BATTLE,
	GAME_CLEAR,
	GAME_OVER,
	GAME_EXIT
};

class GameStateManager : public Instance<GameStateManager>
{
	friend class Instance<GameStateManager>;
private:
	GameState currentState;
	GameState previousState;

	GameStateManager();
	~GameStateManager();
public:
	GameState GetCurrentState() const;
	GameState GetPreviousState() const;
	void ChangeState(GameState newState);
	void SetPreviousState(GameState state);
	void HandleState();
	GameState GetStateFromKey(const char key);
};

#define STATE_MANAGER (GameStateManager::GetInstance())
#define CHANGE_STATE(state) STATE_MANAGER->ChangeState(state)
#define CURRENT_STATE() STATE_MANAGER->GetCurrentState()
#define PREVIOUS_STATE() STATE_MANAGER->GetPreviousState()
#define SET_PREVIOUS_STATE(state) STATE_MANAGER->SetPreviousState(state)
