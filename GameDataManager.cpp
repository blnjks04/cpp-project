#include "GameDataManager.h"
#include "Logger.h"
#include "Player.h"

#include "json.hpp"
#include <fstream>

using json = nlohmann::json;

bool GameDataManager::LoadEnemyData(const std::string& filePath)
{
    std::ifstream inFile(filePath);
    if (!inFile.is_open())
    {
        ADD_LOG(LogType::Error, "Enemy DB 파일 찾기 실패.");
        return false;
    }

    json enemyJsons;
    inFile >> enemyJsons;
    inFile.close();

    EnemyDataBase.clear();
    for (const auto& enemyJson : enemyJsons)
    {
        EnemyData newEnemy;
        newEnemy.rank = GetEnemyRankFromString(enemyJson["rank"]);
        newEnemy.name = enemyJson["name"];
        newEnemy.hp = enemyJson["hp"];
        newEnemy.attack = enemyJson["attack"];

        if (enemyJson.contains("skills"))
        {
            for (const auto& skillJson : enemyJson["skills"])
            {
                Skill newSkill;
                newSkill.name = skillJson["name"];
                newSkill.type = GetSkillTypeFromString(skillJson["type"]);

                if (skillJson.contains("effects"))
                {
                    for (auto effectIt = skillJson["effects"].begin(); effectIt != skillJson["effects"].end(); ++effectIt)
                    {
                        newSkill.skillEffect[GetSkillTypeFromString(effectIt.key())] = effectIt.value();
                    }
                }

                newEnemy.skills.push_back(newSkill);
            }
        }

        for (const auto& spriteLine : enemyJson["sprite"])
        {
            newEnemy.sprite.push_back(spriteLine);
        }

        for (const auto& animationFrame : enemyJson["diedAnim"])
        {
            newEnemy.animSprite["diedAnim"].push_back(animationFrame);
        }

        EnemyDataBase[GetEnemyDataKey(newEnemy.rank, newEnemy.name)] = newEnemy;
    }

    return true;
}

EnemyData* GameDataManager::GetEnemyData(EnemyRank rank, const std::string& enemyName)
{
    auto it = EnemyDataBase.find(GetEnemyDataKey(rank, enemyName));
    if (it != EnemyDataBase.end())
    {
        return &(it->second);
    }
    return nullptr;
}

EnemyRank GameDataManager::GetEnemyRankFromString(const std::string& rankName)
{
    if (rankName == "ELITE") return EnemyRank::ELITE;
    if (rankName == "BOSS") return EnemyRank::BOSS;
    return EnemyRank::NORMAL;
}

SkillType GameDataManager::GetSkillTypeFromString(const std::string& skillTypeName)
{
    if (skillTypeName == "DEFENSE") return SkillType::DEFENSE;
    if (skillTypeName == "BUFF") return SkillType::BUFF;
    if (skillTypeName == "DEBUFF") return SkillType::DEBUFF;
    if (skillTypeName == "SPECIAL") return SkillType::SPECIAL;
    return SkillType::ATTACK;
}

std::string GameDataManager::GetEnemyDataKey(EnemyRank rank, const std::string& enemyName) const
{
    std::string rankName = "NORMAL";
    if (rank == EnemyRank::ELITE) rankName = "ELITE";
    else if (rank == EnemyRank::BOSS) rankName = "BOSS";
    return std::format("{}:{}", rankName, enemyName);
}

bool GameDataManager::LoadPlayerData(const std::string& filePath)
{
    std::ifstream inFile(filePath);
    if (!inFile.is_open())
    {
        ADD_LOG(LogType::Error, "Player DB 파일 찾기 실패.");
        return false;
    }

    json playerJsons;
    inFile >> playerJsons;
    inFile.close();

    if (playerData == nullptr) playerData = new PlayerData();
    playerData->sprite.clear();
    playerData->weaponSprite.clear();

    for (const auto& playerJson : playerJsons)
    {
        playerData->name = playerJson["name"];
        playerData->level = playerJson["level"];
        playerData->hp = playerJson["hp"];
        playerData->currentHp = playerJson.contains("currentHp") ? static_cast<int>(playerJson["currentHp"]) : playerData->hp;
        playerData->attack = playerJson["attack"];
        playerData->gold = playerJson.contains("gold") ? static_cast<int>(playerJson["gold"]) : 0;

        for (const auto& spriteLine : playerJson["sprite"])
        {
            playerData->sprite.push_back(spriteLine);
        }
        for (const auto& weaponSpriteLine : playerJson["weaponSprite"])
        {
            playerData->weaponSprite.push_back(weaponSpriteLine);
        }
    }
    return true;
}

bool GameDataManager::SavePlayerData(const std::string& filePath, Player* player)
{
    if (player == nullptr) return false;

    json playerJson = json::array();
    json savedPlayer;

    savedPlayer["name"] = player->GetName();
    savedPlayer["level"] = player->GetLevel();
    savedPlayer["hp"] = player->Status->GetMaxHp();
    savedPlayer["currentHp"] = player->Status->GetCurrentHp();
    savedPlayer["attack"] = player->GetBaseDamage();
    savedPlayer["gold"] = player->GetGold();
    savedPlayer["sprite"] = player->GetSprite();
    savedPlayer["weaponSprite"] = player->GetWeaponSprite();
    playerJson.push_back(savedPlayer);

    std::ofstream outFile(filePath);
    if (!outFile.is_open())
    {
        ADD_LOG(LogType::Error, "Player DB 파일 저장 실패.");
        return false;
    }

    outFile << playerJson.dump(2);
    outFile.close();
    return true;
}

PlayerData* GameDataManager::GetPlayerData(const std::string& name)
{
    if (playerData != nullptr && playerData->name == name) return playerData;
    return nullptr;
}

bool GameDataManager::LoadEventStageData(const std::string& filePath)
{
    std::ifstream inFile(filePath);
    if (!inFile.is_open())
    {
        ADD_LOG(LogType::Error, "EventStage DB 파일 찾기 실패.");
        return false;
    }

    json eventJson;
    inFile >> eventJson;
    inFile.close();

    EventStageDataBase.clear();
    for (const auto& stageJson : eventJson)
    {
        EventStage newStage;
        newStage.stageName = stageJson["stageName"];
        for (const auto& descriptionLine : stageJson["description"])
        {
            newStage.description.push_back(descriptionLine);
        }

        for (const auto& choiceJson : stageJson["choices"])
        {
            EventChoice newChoice;
            newChoice.text = choiceJson["text"];
            newChoice.hpChange = choiceJson["hpChange"];
            newChoice.maxHpChange = choiceJson["maxHpChange"];
            newChoice.bIsLevelUp = choiceJson["isLevelUp"];
            newChoice.bIsBattle = choiceJson["isBattle"];
            newChoice.bIsPokerBattle = choiceJson.contains("isPokerBattle") ? static_cast<bool>(choiceJson["isPokerBattle"]) : false;
            newChoice.resultText = choiceJson["resultText"];
            newChoice.nextState = GetGameStateFromString(choiceJson["nextState"]);
            newStage.choices.push_back(newChoice);
        }

        EventStageDataBase.push_back(newStage);
    }

    ADD_LOG(LogType::StateChange, std::format("EventStage DB 로드 완료: {}", EventStageDataBase.size()));
    return !EventStageDataBase.empty();
}

const std::vector<EventStage>& GameDataManager::GetEventStages() const
{
    return EventStageDataBase;
}

GameState GameDataManager::GetGameStateFromString(const std::string& stateName)
{
    if (stateName == "BATTLE") return GameState::BATTLE;
    if (stateName == "ELITE_BATTLE") return GameState::ELITE_BATTLE;
    if (stateName == "BOSS_BATTLE") return GameState::BOSS_BATTLE;
    if (stateName == "GAME_OVER") return GameState::GAME_OVER;
    return GameState::MAP;
}
