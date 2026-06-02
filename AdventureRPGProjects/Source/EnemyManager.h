#pragma once
#include "Utils.h"
#include "Enemy.h"
#include "GameDataManager.h"

class EnemyManager
{
private:
    // EnemyManager가 적 객체의 실제 소유권과 전투용 포인터 목록을 함께 관리한다.
    std::vector<std::unique_ptr<Enemy>> ownedEnemies;
    std::vector<Enemy*> enemies;
    std::string GetRandomEnemyNameByFloor(EnemyRank rank, int floor);
    void CreateEnemyByRankAndName(EnemyRank rank, const std::string& enemyName);
    int GetScaledHp(int baseHp, int floor);
    int GetScaledAttack(int baseAttack, int floor);

public:
    EnemyManager() = default;
    ~EnemyManager() = default;

    void CreateEnemy();
    void CreateEliteEnemy();
    void CreateBoss();
    void DeleteEnemies();

    std::vector<Enemy*>& GetEnemies();
};
