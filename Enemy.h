#pragma once
#include "Utils.h"
#include "Character.h"
#include "StatusComponent.h"
#include "AbilityComponent.h"
#include "EnemyFSM.h"

class Player;

struct Rect
{
    int left;
    int right;
    int top;
    int bottom;
};

enum class SkillType
{
    ATTACK,
    DEFENSE,
    BUFF,
    DEBUFF,
    SPECIAL
};

struct Skill
{
    std::string name;
    SkillType type;
    std::unordered_map<SkillType, int> skillEffect;
};

// 적 클래스
class Enemy : public Character
{
private:
	int level;
	int baseDamage;
    std::string lastActionMessage;
    std::vector<std::string> sprite;
    Rect spriteRect;
    
    std::unique_ptr<EnemyFSM> FSM;
    EnemyState currentState;
    std::vector<Skill> skillList;
    Skill skill;
    std::unordered_map<std::string, std::vector<std::vector<std::string>>> animSprite;

protected:
    void SetLastActionMessage(const std::string& message);

public:
	Enemy(const std::string& name, int level = 1, EnemyType enemyType = EnemyType::NORMAL);
	virtual ~Enemy();

	StatusComponent* Status;
	AbilityComponent* Ability;

	int GetBaseDamage() const;
    std::vector<std::string>& GetSprite();
    void SetSprite(const std::vector<std::string>& newSprite);
    void SetAnimSprite(std::unordered_map<std::string, std::vector<std::vector<std::string>>> newAnim);
    std::vector<std::vector<std::string>>& GetAnimSprite(const std::string& name);
    void SetBaseDamage(int damage);
    void SetSpriteRect(const Rect& rect);
    Rect* GetSpriteRect();
	virtual void DoAttack(Player* player);
    virtual void ExecuteFSM(Player* player);
    virtual void DoSkill(Player* player);
    std::string GetLastActionMessage() const;
	virtual void TakeDamage(int damage) override;
    virtual std::string GetNextAction();
    virtual void SetNextAction();
    void AddSkill(Skill skill);
    Skill GetRandomSkill();
};
