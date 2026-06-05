#include "Game.h"
#include "GameStateManager.h"
#include "UIManager.h"
#include "MapManager.h"
#include "Logger.h"
#include "CombatView.h"
#include "Enemy.h"
#include "EnemyManager.h"
#include "CombatManager.h"
#include "GameDataManager.h"
#include "EventStageManager.h"
#include "Player.h"
#include "ButtonRenderer.h"
#include "MainMenuButtons.h"
#include "PlayerInfoView.h"


Game::~Game()
{
	ADD_LOG(LogType::ALL, "Game Exited.");
}

void Game::Init()
{
	CONSOLE->Init();
	ADD_LOG(LogType::ALL, "Game initialized.");
    GameDataManager::GetInstance()->LoadEnemyData("DB_Enemy.json");
    GameDataManager::GetInstance()->LoadEventStageData("DB_EventStage.json");
    GameDataManager::GetInstance()->LoadPlayerData("DB_Player.json");
    player = std::make_unique<Player>("player");
}

void Game::Run()
{
	while (CURRENT_STATE() != GameState::GAME_EXIT)
	{
		Update();
	}
	CLEAR_SCREEN();
}

void Game::Update()
{
    if (player->Status->GetCurrentHp() <= 0) CHANGE_STATE(GameState::GAME_OVER);
	switch (CURRENT_STATE())
	{
	case GameState::MENU:
		if (PREVIOUS_STATE() != GameState::MENU)
		{
            CLEAR_SCREEN();
			uiManager.DisplayMainMenu();
		}
		click = GET_MOUSE_CLICK();
		SET_PREVIOUS_STATE(CURRENT_STATE());
		if (ButtonRenderer::IsClicked(MainMenuButtons::Start(), click))
		{
			CHANGE_STATE(GameState::MAP);
		}
		else if (ButtonRenderer::IsClicked(MainMenuButtons::Rules(), click))
		{
            CLEAR_SCREEN();
            uiManager.DisplayGameRules();
            GET_MOUSE_CLICK();
            SET_PREVIOUS_STATE(GameState::NONE);
		}
		else if (ButtonRenderer::IsClicked(MainMenuButtons::Exit(), click))
		{
			CHANGE_STATE(GameState::GAME_EXIT);
		}
			
		break;
	case GameState::MAP:
        if (PREVIOUS_STATE() != GameState::MAP)
        {
            CLEAR_SCREEN();
            MapManager::GetInstance()->DisplayMap();
        }
		SET_PREVIOUS_STATE(CURRENT_STATE());
        click = GET_MOUSE_CLICK();
        if (MapManager::GetInstance()->IsClickedNextNode(click))
        {
            MapManager::GetInstance()->MovePlayer();
        }
        else if (MapManager::GetInstance()->IsClickedPlayerInfoBox(click))
        {
            if (GameDataManager::GetInstance()->LoadPlayerData("DB_Player.json"))
            {
                PlayerData* loadedPlayerData = GameDataManager::GetInstance()->GetPlayerData(player->GetName());
                if (loadedPlayerData != nullptr)
                {
                    player->ApplyData(*loadedPlayerData);
                    CLEAR_SCREEN();
                    PlayerInfoView playerInfoView;
                    playerInfoView.DisplayPlayerInfo(player.get());
                    GET_MOUSE_CLICK();
                    CLEAR_SCREEN();
                    MapManager::GetInstance()->DisplayMap();
                }
            }
        }
		break;

	case GameState::EVENT:
		if (PREVIOUS_STATE() != GameState::EVENT)
		{
            CLEAR_SCREEN();
            SET_PREVIOUS_STATE(CURRENT_STATE());
            eventStageManager.StartEvent(player.get());
        }
		break;
	case GameState::REST:
        if (PREVIOUS_STATE() != GameState::REST)
        {
            CLEAR_SCREEN();
            SET_PREVIOUS_STATE(CURRENT_STATE());
            uiManager.DisplayRestUI(player.get());
        }
        click = GET_MOUSE_CLICK();
        if (uiManager.IsClickedRestButton(click))
        {
            int healAmount = player->Status->GetMaxHp() * 35 / 100;
            if (healAmount < 1) healAmount = 1;
            int nextHp = player->Status->GetCurrentHp() + healAmount;
            if (nextHp > player->Status->GetMaxHp()) nextHp = player->Status->GetMaxHp();
            player->Status->SetCurrentHp(nextHp);

            uiManager.DisplayRestResult(player.get(), std::format("휴식으로 체력을 {} 회복했습니다.", healAmount));
            GET_MOUSE_CLICK();
            CHANGE_STATE(GameState::MAP);
        }
        else if (uiManager.IsClickedAbilityUpgradeButton(click))
        {

            player->AddBaseDamage(1);
            uiManager.DisplayRestResult(player.get(), "능력 강화로 공격력이 1 증가했습니다.");
            GET_MOUSE_CLICK();
            CHANGE_STATE(GameState::MAP);
        }
        break;
	case GameState::BATTLE:
		if (PREVIOUS_STATE() != GameState::BATTLE)
		{
            CLEAR_SCREEN();
            SET_PREVIOUS_STATE(CURRENT_STATE());
            enemyManager.DeleteEnemies();
            enemyManager.CreateEnemy();
            std::vector<Enemy*>& enemies = enemyManager.GetEnemies();
            combatManager.StartBattle(player.get(), enemies);
			
		}
		break;
	case GameState::ELITE_BATTLE:
        if (PREVIOUS_STATE() != GameState::ELITE_BATTLE)
        {
            CLEAR_SCREEN();
            SET_PREVIOUS_STATE(CURRENT_STATE());
            enemyManager.DeleteEnemies();
            enemyManager.CreateEliteEnemy();
            std::vector<Enemy*>& enemies = enemyManager.GetEnemies();
            combatManager.StartBattle(player.get(), enemies);
        }
		break;
	case GameState::BOSS_BATTLE:
        if (PREVIOUS_STATE() != GameState::BOSS_BATTLE)
        {
            CLEAR_SCREEN();
            SET_PREVIOUS_STATE(CURRENT_STATE());
            enemyManager.DeleteEnemies();
            enemyManager.CreateBoss();
            std::vector<Enemy*>& enemies = enemyManager.GetEnemies();
            combatManager.StartBattle(player.get(), enemies);
        }
		break;
	case GameState::GAME_EXIT:
		CLEAR_SCREEN();
		break;
	case GameState::GAME_OVER:
		CLEAR_SCREEN();
		PRINT_TEXT(2, 2, "GAME OVER");
		PRINT_TEXT(2, 4, "[ 게임 종료 ] 버튼을 클릭하세요.", "red");
		GET_MOUSE_CLICK();
		CHANGE_STATE(GameState::GAME_EXIT);
		break;
	}

    /*
    if (player != nullptr)
    {
        GameDataManager::GetInstance()->SavePlayerData("DB_Player.json", player.get());
    }
    */
}

void Game::Render(const std::string& text, int x, int y)
{
	CLEAR_SCREEN();
	PRINT_TEXT(x, y, text);
}
 
void Game::Cleanup()
{
}
