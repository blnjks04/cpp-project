#pragma once
#include "Utils.h"
#include "Instance.h"
#include "EventStage.h"
#include "Enemy.h"

class Player;

enum class EnemyRank
{
    NORMAL,
    ELITE,
    BOSS
};

struct EnemyData
{
    EnemyRank rank;
    std::string name;
    int hp;
    int attack;
    std::vector<Skill> skills;
    std::vector<std::string> sprite;
    std::unordered_map<std::string, std::vector<std::vector<std::string>>> animSprite;
};

struct PlayerData
{
    std::string name;
    int level;
    int hp;
    int currentHp = 0;
    int attack;
    int gold = 0;
    std::vector<std::string> sprite;
    std::vector<std::string> weaponSprite;
};

class GameDataManager : public Instance<GameDataManager>
{
    friend class Instance<GameDataManager>;
private:
    GameDataManager() {};
    ~GameDataManager() {};

    std::unordered_map<std::string, EnemyData> EnemyDataBase;
    std::vector<EventStage> EventStageDataBase;
    PlayerData* playerData = nullptr;
    GameState GetGameStateFromString(const std::string& stateName);
    EnemyRank GetEnemyRankFromString(const std::string& rankName);
    SkillType GetSkillTypeFromString(const std::string& skillTypeName);
    std::string GetEnemyDataKey(EnemyRank rank, const std::string& enemyName) const;

public:
    bool LoadEnemyData(const std::string& filePath);
    EnemyData* GetEnemyData(EnemyRank rank, const std::string& enemyName);
    bool LoadPlayerData(const std::string& filePath);
    bool SavePlayerData(const std::string& filePath, Player* player);
    PlayerData* GetPlayerData(const std::string& playerName);
    bool LoadEventStageData(const std::string& filePath);
    const std::vector<EventStage>& GetEventStages() const;
    bool GameSave();
    bool GameLoad();
};
