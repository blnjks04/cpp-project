#include "EnemyFSM.h"
#include "Enemy.h"
#include "Player.h"
#include "GameDataManager.h"



void EnemyFSM::ExecuteAction(Player* player)
{

    switch (currentState)
    {
    case EnemyState::IDLE:
        EnterIdle();
        break;
    case EnemyState::ATTACK:
        EnterAttack(player);
        break;
    case EnemyState::SKILL:
        EnterSkill(player);
        break;
    case EnemyState::DIED:
        EnterDied();
        break;
    case EnemyState::PHASE_2:
        UpdatePhase();
        break;
    default:
        break;
    }

    turnCount++;
    currentState = EnemyState::IDLE;
    EnterIdle();
}

void EnemyFSM::ChangeState(EnemyState newState)
{
    currentState = newState;
}

void EnemyFSM::UpdatePhase()
{
    
}

EnemyState EnemyFSM::GetCurrentState()
{
    return currentState;
}

EnemyState EnemyFSM::DecideNextState()
{
    if (!owner->Status->IsAlive()) return EnemyState::DIED;

    if (type == EnemyType::NORMAL) return EnemyState::ATTACK;
    if (type == EnemyType::ELITE)
    {
        return (turnCount % 3 == 2) ? EnemyState::SKILL : EnemyState::ATTACK;
    }
    if (type == EnemyType::BOSS)
    {
        if (owner->Status->GetCurrentHp() < 30)
        {
            return EnemyState::PHASE_2;
        }
        return (turnCount % 2 == 1) ? EnemyState::SKILL : EnemyState::ATTACK;
    }

    return EnemyState::ATTACK;
}

void EnemyFSM::EnterIdle()
{

}

void EnemyFSM::ExecuteIdle()
{

}

void EnemyFSM::EnterAttack(Player* player)
{
    ExecuteAttack(player);
}

void EnemyFSM::ExecuteAttack(Player* player)
{
    owner->DoAttack(player);
}

void EnemyFSM::EnterSkill(Player* player)
{
    if (type == EnemyType::NORMAL)
    {
        EnterAttack(player);
        return;
    }
    ExecuteSkill(player);
}

void EnemyFSM::ExecuteSkill(Player* player)
{
    owner->DoSkill(player);
}

void EnemyFSM::EnterDied()
{

}

void EnemyFSM::ExecuteDied()
{

}
