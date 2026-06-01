#pragma once
#include "Utils.h"
#include "Character.h"
#include "StatusComponent.h"
#include "AbilityComponent.h"

class Enemy;
struct PlayerData;

// 플레이어 클래스
class Player : public Character
{
private:
	int level = 1;
	int baseDamage = 8;
	int rerolls = 3;
    int currentBlock = 0;
	int gold = 0;
    std::vector<std::string> sprite;
    std::vector<std::string> weaponSprite;
    int actionPoint = 0;

public:
	Player(const std::string& name);
	~Player();

	StatusComponent* Status;
	AbilityComponent* Ability;

	int GetLevel();
	int GetGold();
	int GetBaseDamage() const;
    int GetRerolls();
    int GetActionPoint();
    void ApplyData(const PlayerData& playerData);
    void SetActionPoint(int point);
    void SetRerolls(int newReroll);
    void AddBaseDamage(int amount);
	void DoAttack(Enemy* Enemy, int bonusDamage = 0);
	void GainBlock(int amount);
    void GainGold(int amount);
	virtual void TakeDamage(int damage) override;
	void LevelUp();
	void UseReroll();
    std::vector<std::string>& GetSprite();
    std::vector<std::string>& GetWeaponSprite();

	// void GainExp(int exp);
};
