#pragma once
class Enemy;
class Player;
enum class EnemyType
{
    NORMAL,
    ELITE,
    BOSS
};

enum class EnemyState
{
    IDLE,
    ATTACK,
    SKILL,
    DIED,
    PHASE_2
};

class EnemyFSM
{
private:
    Enemy* owner;
    EnemyType type;
    EnemyState currentState;

    int turnCount = 0;
    int phase = 1;
    int skillCooldown = 0;

public:
    EnemyFSM(Enemy* owner, EnemyType type) : owner(owner), type(type), currentState(EnemyState::IDLE) {};
    ~EnemyFSM() = default;

    void ExecuteAction(Player* player);
    void ChangeState(EnemyState newState);
    EnemyState DecideNextState();
    EnemyState GetCurrentState();
private:
    void UpdatePhase();

    void EnterIdle();
    void ExecuteIdle();

    void EnterAttack(Player* player);
    void ExecuteAttack(Player* player);

    void EnterSkill(Player* player);
    void ExecuteSkill(Player* player);

    void EnterDied();
    void ExecuteDied();
};
