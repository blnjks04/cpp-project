#include "EnemyManager.h"
#include "BossEnemy.h"
#include "MapManager.h"
#include "Logger.h"

void EnemyManager::CreateEnemy()
{
    int floor = MapManager::GetInstance()->GetCurrentFloor();
    CreateEnemyByRankAndName(EnemyRank::NORMAL, GetRandomEnemyNameByFloor(EnemyRank::NORMAL, floor));
}

void EnemyManager::CreateEliteEnemy()
{
    int floor = MapManager::GetInstance()->GetCurrentFloor();
    CreateEnemyByRankAndName(EnemyRank::ELITE, GetRandomEnemyNameByFloor(EnemyRank::ELITE, floor));
}

void EnemyManager::CreateBoss()
{
    CreateEnemyByRankAndName(EnemyRank::BOSS, GetRandomEnemyNameByFloor(EnemyRank::BOSS, MapManager::GetInstance()->GetCurrentFloor()));
}

void EnemyManager::CreateEnemyByRankAndName(EnemyRank rank, const std::string& enemyName)
{
    int floor = MapManager::GetInstance()->GetCurrentFloor();

    EnemyData* enemyData = GameDataManager::GetInstance()->GetEnemyData(rank, enemyName);
    if (enemyData == nullptr)
    {
        ADD_LOG(LogType::Error, std::format("적 생성 실패: rank/name 데이터 없음 ({})", enemyName));
        return;
    }

    EnemyType enemyType = EnemyType::NORMAL;
    if (rank == EnemyRank::ELITE) enemyType = EnemyType::ELITE;
    else if (rank == EnemyRank::BOSS) enemyType = EnemyType::BOSS;

    std::unique_ptr<Enemy> enemy;
    if (rank == EnemyRank::BOSS) enemy = std::make_unique<BossEnemy>(enemyData->name, 1);
    else enemy = std::make_unique<Enemy>(enemyData->name, 1, enemyType);

    Enemy* enemyPtr = enemy.get();
    int scaledHp = GetScaledHp(enemyData->hp, floor);
    int scaledAttack = GetScaledAttack(enemyData->attack, floor);
    enemyPtr->Status->SetMaxHp(scaledHp);
    enemyPtr->Status->SetCurrentHp(scaledHp);
    enemyPtr->SetBaseDamage(scaledAttack);
    enemyPtr->SetSprite(enemyData->sprite);
    enemyPtr->SetAnimSprite(enemyData->animSprite);

    for (const auto& skill : enemyData->skills)
    {
        enemyPtr->AddSkill(skill);
    }

    int maxWidth = 0;
    for (auto& spriteLine : enemyPtr->GetSprite())
    {
        if (maxWidth < static_cast<int>(spriteLine.size())) maxWidth = static_cast<int>(spriteLine.size());
    }

    Rect newRect;
    newRect.top = 3;
    newRect.bottom = newRect.top + static_cast<int>(enemyPtr->GetSprite().size());
    newRect.left = 72 + (enemies.empty() ? 0 : static_cast<int>(enemies.size()) * 20);
    newRect.right = newRect.left + maxWidth;
    enemyPtr->SetSpriteRect(newRect);

    enemies.push_back(enemyPtr);
    ownedEnemies.push_back(std::move(enemy));
    ADD_LOG(LogType::ALL, std::format("적 생성 성공: floor {}, name {}, hp {}, attack {}", floor, enemyName, scaledHp, scaledAttack));
}

std::string EnemyManager::GetRandomEnemyNameByFloor(EnemyRank rank, int floor)
{
    std::vector<std::string> enemyNames;
    if (rank == EnemyRank::BOSS)
    {
        enemyNames = { "JokerKing" };
    }
    else if (rank == EnemyRank::ELITE)
    {
        // 엘리트 비숍 데이터를 추가했으므로 엘리트 전투 후보에 포함한다.
        if (floor <= 7) enemyNames = { "Rook", "Knight", "Bishop" };
        else enemyNames = { "Rook", "Knight", "Bishop", "Queen" };
    }
    else if (floor <= 3)
    {
        enemyNames = { "Pawn", "Pawn", "Bishop" };
    }
    else if (floor <= 7)
    {
        enemyNames = { "Pawn", "Rook", "Knight", "Bishop" };
    }
    else if (floor <= 11)
    {
        enemyNames = { "Rook", "Knight", "Bishop", "Queen" };
    }
    else
    {
        enemyNames = { "Knight", "Queen", "Queen", "King" };
    }

    std::random_device randomDevice;
    std::mt19937 randomGenerator(randomDevice());
    std::uniform_int_distribution<int> enemyNameDistribution(0, static_cast<int>(enemyNames.size()) - 1);
    return enemyNames[enemyNameDistribution(randomGenerator)];
}

int EnemyManager::GetScaledHp(int baseHp, int floor)
{
    int tier = floor / 4;
    return baseHp + (floor * 3) + (tier * 5);
}

int EnemyManager::GetScaledAttack(int baseAttack, int floor)
{
    int tier = floor / 4;
    return baseAttack + (floor / 2) + tier;
}

std::vector<Enemy*>& EnemyManager::GetEnemies()
{
    return enemies;
}

void EnemyManager::DeleteEnemies()
{
    ownedEnemies.clear();
    enemies.clear();
}
