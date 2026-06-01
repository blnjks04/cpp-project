#include "Utils.h"
#include "Player.h"
#include "Enemy.h"
#include "EventDispatcher.h"
#include "Logger.h"
#include "GameDataManager.h"

Player::Player(const std::string& name) : Character(name)
{
	
	AddComponent(std::make_unique<StatusComponent>(this));
	AddComponent(std::make_unique<AbilityComponent>(this));

	Status = GetComponent<StatusComponent>();
	Ability = GetComponent<AbilityComponent>();

	EventDispatcher::GetInstance()->Bind("OnAttack", this, [this](const DamageEvent event)
		{
			if (event.target == this)
			{
				this->TakeDamage(event.damage);
			}
		});

    Status->SetBlock(0);
    PlayerData* playerData = GameDataManager::GetInstance()->GetPlayerData(name);
    if (playerData != nullptr)
    {
        ApplyData(*playerData);
    }
}

Player::~Player()
{
	EventDispatcher::GetInstance()->Unbind("OnAttack", this);
}

int Player::GetLevel()
{
	return level;
}

int Player::GetGold()
{
	return gold;
}

int Player::GetBaseDamage() const
{
	return baseDamage;
}

int Player::GetActionPoint()
{
    return actionPoint;
}

void Player::ApplyData(const PlayerData& playerData)
{
    level = playerData.level;
    baseDamage = playerData.attack;
    gold = playerData.gold;
    sprite = playerData.sprite;
    weaponSprite = playerData.weaponSprite;

    int maxHp = playerData.hp;
    if (maxHp < 1) maxHp = 1;
    Status->SetMaxHp(maxHp);

    int nextHp = playerData.currentHp > 0 ? playerData.currentHp : maxHp;
    if (nextHp > maxHp) nextHp = maxHp;
    if (nextHp < 0) nextHp = 0;
    Status->SetCurrentHp(nextHp);
    Status->SetBIsAlive(nextHp > 0);
}

void Player::SetActionPoint(int point)
{
    actionPoint = point;
}

void Player::AddBaseDamage(int amount)
{
    baseDamage += amount;
    if (baseDamage < 1) baseDamage = 1;
}

int Player::GetRerolls()
{
    return rerolls;
}

void Player::SetRerolls(int newReroll)
{
    if (newReroll < 0) newReroll = 0;
    rerolls = newReroll;
}

void Player::GainGold(int amount)
{
    gold += amount;
}
/*
void Player::GainExp(int exp)
{
	currentExp += exp;
	ADD_LOG(LogType::ALL, std::format("경험치 흭득 :+{}. 경험치 : {}/{}", exp, currentExp, maxExp));

	if (currentExp >= maxExp)
	{
		currentExp -= maxExp;
		LevelUp();
	}
}*/

void Player::LevelUp()
{
	level++;
	ADD_LOG(LogType::ALL, std::format("플레이어 레벨업! 레벨 : {}", level));
	
    // 전투 승리마다 레벨업하는 현재 구조에 맞춰 성장폭을 완만하게 조정한다.
	Status->SetMaxHp(Status->GetMaxHp() + 4);
    int nextHp = Status->GetCurrentHp() + 6;
    if (nextHp > Status->GetMaxHp()) nextHp = Status->GetMaxHp();
	Status->SetCurrentHp(nextHp);
	baseDamage += 1;

}

void Player::UseReroll()
{
    if (rerolls > 0) rerolls--;
}

void Player::DoAttack(Enemy* enemy, int bonusDamage)
{
	int damage = baseDamage + bonusDamage;
	DamageEvent attackEvent(this, enemy, damage);
	EventDispatcher::GetInstance()->BroadCast("OnAttack", attackEvent);
	ADD_LOG(LogType::BattleEvent, std::format("플레이어가 {}에게 {} 피해로 공격했습니다.", enemy->GetName(), damage));
	
}

void Player::GainBlock(int amount)
{
	if (amount <= 0) return;
	Status->SetBlock(Status->GetBlock() + amount);
	ADD_LOG(LogType::BattleEvent, std::format("플레이어가 방어막 {}을 얻었습니다. 현재 방어막 : {}", amount, Status->GetBlock()));
}

void Player::TakeDamage(int damage)
{
	int block = Status->GetBlock();
	if (block > 0)
	{
		if (block >= damage)
		{
			Status->SetBlock(block - damage);
            ADD_LOG(LogType::ALL, std::format("플레이어가 {}의 피해를 막았습니다. 남은 방어막 : {}", damage, Status->GetBlock()));
		}
		if (block >= damage) return;
        else 
        {
            Status->SetBlock(0);
            damage -= block;
        }
		ADD_LOG(LogType::ALL, std::format("플레이어가 {}의 피해를 막았습니다. 남은 방어막 : {}", block, Status->GetBlock()));
	}
	
	int currentHp = Status->GetCurrentHp();
	currentHp -= damage;
	if (currentHp < 0) currentHp = 0;
	Status->SetCurrentHp(currentHp);
	ADD_LOG(LogType::ALL, std::format("플레이어가 {}의 피해를 입었습니다. 남은 HP : {}/{}", damage, Status->GetCurrentHp(), Status->GetMaxHp()));
	if (currentHp == 0)
	{
		ADD_LOG(LogType::ALL, "플레이어가 사망했습니다.");
        Status->SetBIsAlive(false);
	}
	
}

std::vector<std::string>& Player::GetSprite()
{
    return sprite;
}

std::vector<std::string>& Player::GetWeaponSprite()
{
    return weaponSprite;
}

