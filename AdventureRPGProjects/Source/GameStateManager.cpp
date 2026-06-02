#include "Game.h"
#include "GameStateManager.h"
#include "Logger.h"
GameStateManager::GameStateManager()
{
	currentState = GameState::MENU;
	previousState = GameState::NONE;
}

GameStateManager::~GameStateManager()
{

}

void GameStateManager::ChangeState(GameState newState)
{
	currentState = newState;
	ADD_LOG(LogType::StateChange, std::format("State changed to: {}", static_cast<int>(currentState)));
}

void GameStateManager::SetPreviousState(GameState state)
{
	previousState = state;
	ADD_LOG(LogType::StateChange, std::format("Previous state set to: {}", static_cast<int>(previousState)));
}

GameState GameStateManager::GetCurrentState() const
{
	return currentState;
}

GameState GameStateManager::GetPreviousState() const
{
	return previousState;
}

GameState GameStateManager::GetStateFromKey(const char key)
{
	switch (key)
	{
	case '1':
		return GameState::MAP;
	case '2':
		return GameState::EVENT;
	case 'b':
	case 'B':
		return GameState::BATTLE;
	case '3':
		return GameState::GAME_EXIT;
	case '0':
		return GameState::GAME_EXIT;
	default:
		return currentState; 
	}
}

void GameStateManager::HandleState()
{

}
