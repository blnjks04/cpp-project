#include "Enemy.h"
#include "Logger.h"
#include "EventDispatcher.h"
#include "Player.h"
#include "EnemyFSM.h"

Enemy::Enemy(const std::string& name, int level, EnemyType enemyType)
    : Character(name), level(level), baseDamage(8 + level * 2), spriteRect{}, FSM(std::make_unique<EnemyFSM>(this, enemyType))
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
    
}

Enemy::~Enemy()
{
	EventDispatcher::GetInstance()->Unbind("OnAttack", this);
}

int Enemy::GetBaseDamage() const
{
	return baseDamage;
}

void Enemy::DoAttack(Player* player)
{
	DamageEvent attackEvent(this, player, baseDamage);
	EventDispatcher::GetInstance()->BroadCast("OnAttack", attackEvent);
	lastActionMessage = std::format("{}가 플레이어를 {} 피해로 공격했습니다.", GetName(), baseDamage);
	ADD_LOG(LogType::BattleEvent, lastActionMessage);
}

std::string Enemy::GetNextAction()
{
    if (FSM->GetCurrentState() == EnemyState::SKILL)
    {
        switch (skill.type)
        {
        case SkillType::ATTACK:
            return std::format("🗡️  {}", skill.skillEffect[skill.type]);
        case SkillType::DEFENSE:
            return std::format("🛡️  {}", skill.skillEffect[skill.type]);
        case SkillType::BUFF:
            return std::format("▲");
        case SkillType::DEBUFF:
            return std::format("▼");
        case SkillType::SPECIAL:
            return "❓";
        }
    }
    return std::format("🗡️  {}", GetBaseDamage());
}

void Enemy::SetNextAction()
{
    FSM->ChangeState(FSM->DecideNextState());
    if (FSM->GetCurrentState() == EnemyState::SKILL)
    {
        skill = GetRandomSkill();
    }
}

void Enemy::ExecuteFSM(Player* player)
{
    if (FSM == nullptr) return;
    FSM->ExecuteAction(player);
}

void Enemy::DoSkill(Player* player)
{
    if (skill.name.empty())
    {
        DoAttack(player);
        return;
    }

    lastActionMessage = std::format("{}가 스킬 [{}]을 사용했습니다.", GetName(), skill.name);
    
    bool bHasAttackEffect = false;
    bool bHasAnyEffect = false;
    for (const auto& effect : skill.skillEffect)
    {
        bHasAnyEffect = true;
        SkillType effectType = effect.first;
        int value = effect.second;

        switch (effectType)
        {
        case SkillType::ATTACK:
        {
            bHasAttackEffect = true;
            DamageEvent attackEvent(this, player, value);
            EventDispatcher::GetInstance()->BroadCast("OnAttack", attackEvent);
            lastActionMessage.append(std::format(" ( {} 피해 )", attackEvent.damage));
            ADD_LOG(LogType::BattleEvent, std::format("{}가 스킬 [{}]로 플레이어에게 {} 피해를 입혔습니다.", GetName(), skill.name, value));
            break;
        }
        case SkillType::DEFENSE:
            Status->SetBlock(Status->GetBlock() + value);
            lastActionMessage.append(std::format(" ( {} 방어막 )", Status->GetBlock()));
            ADD_LOG(LogType::BattleEvent, std::format("{}가 스킬 [{}]로 방어막 {}을 얻었습니다. 현재 방어막 : {}", GetName(), skill.name, value, Status->GetBlock()));
            break;
        case SkillType::BUFF:
            SetBaseDamage(GetBaseDamage() + value);
            lastActionMessage.append(std::format(" ( {} 공격력 {} 증가 )", GetName(), value));
            ADD_LOG(LogType::BattleEvent, std::format("{}가 스킬 [{}]로 공격력 {}을 얻었습니다. 현재 공격력 : {}", GetName(), skill.name, value, GetBaseDamage()));
            break;
        case SkillType::DEBUFF:
            player->AddBaseDamage(-value);
            lastActionMessage.append(std::format(" (플레이어 공격력 {} 감소 )", value));
            ADD_LOG(LogType::BattleEvent, std::format("{}가 스킬 [{}]로 플레이어의 공격력을 {} 낮췄습니다. 플레이어 공격력 : {}", GetName(), skill.name, value, player->GetBaseDamage()));
            break;
        case SkillType::SPECIAL:
            ADD_LOG(LogType::BattleEvent, std::format("{}가 특수 스킬 [{}]을 발동했습니다.", GetName(), skill.name));
            break;
        default:
            break;
        }
    }

    if (!bHasAnyEffect)
    {
        switch (skill.type)
        {
        case SkillType::ATTACK:
            DoAttack(player);
            break;
        case SkillType::DEFENSE:
            Status->SetBlock(Status->GetBlock() + baseDamage);
            ADD_LOG(LogType::BattleEvent, std::format("{}가 스킬 [{}]로 방어막 {}을 얻었습니다.", GetName(), skill.name, baseDamage));
            break;
        case SkillType::BUFF:
            SetBaseDamage(GetBaseDamage() + 1);
            ADD_LOG(LogType::BattleEvent, std::format("{}가 스킬 [{}]로 공격력 1을 얻었습니다.", GetName(), skill.name));
            break;
        case SkillType::DEBUFF:
            player->AddBaseDamage(-1);
            ADD_LOG(LogType::BattleEvent, std::format("{}가 스킬 [{}]로 플레이어의 공격력을 1 낮췄습니다.", GetName(), skill.name));
            break;
        case SkillType::SPECIAL:
            DoAttack(player);
            break;
        }
        return;
    }

    if (!bHasAttackEffect)
    {
        ADD_LOG(LogType::BattleEvent, std::format("{}가 스킬 [{}]을 사용했습니다.", GetName(), skill.name));
    }
}

void Enemy::TakeDamage(int damage)
{
	int block = Status->GetBlock();
	if (block > 0)
	{
		if (block >= damage)
		{
			Status->SetBlock(block - damage);
		}
		else
		{
			Status->SetBlock(0);
			damage -= block;
		}
		ADD_LOG(LogType::ALL, std::format("{}가 {}의 피해를 막았습니다. 남은 방어막 : {}", GetName(), damage, Status->GetBlock()));
		if (block >= damage) return;
	}

	int currentHp = Status->GetCurrentHp();
	currentHp -= damage;
	if (currentHp < 0) currentHp = 0;
	Status->SetCurrentHp(currentHp);
	ADD_LOG(LogType::ALL, std::format("{}가 {}의 피해를 입었습니다. 남은 HP : {}/{}", GetName(), damage, Status->GetCurrentHp(), Status->GetMaxHp()));
	if (currentHp == 0)
	{
		ADD_LOG(LogType::ALL, std::format("{}가 사망했습니다.", GetName()));
        Status->SetBIsAlive(false);
	}
}

std::vector<std::string>& Enemy::GetSprite()
{
    return sprite;
}

void Enemy::SetSprite(const std::vector<std::string>& newSprite)
{
    sprite.clear();
    for (const auto& spriteLine : newSprite)
    {
        sprite.push_back(spriteLine);
    }
}

void Enemy::SetBaseDamage(int damage)
{
    baseDamage = damage;
}

void Enemy::SetSpriteRect(const Rect& rect)
{
    spriteRect = rect;
}

std::string Enemy::GetLastActionMessage() const
{
    return lastActionMessage;
}

void Enemy::SetLastActionMessage(const std::string& message)
{
    lastActionMessage = message;
}

Rect* Enemy::GetSpriteRect()
{
    return &spriteRect;
}

void Enemy::AddSkill(Skill skill)
{
    skillList.push_back(skill);
}

Skill Enemy::GetRandomSkill()
{
    if (skillList.empty()) return {};
    std::random_device randomDevice;
    std::mt19937 randomGenerator(randomDevice());
    std::uniform_int_distribution<int> skillIndexDistribution(0, static_cast<int>(skillList.size()) - 1);

    return skillList[skillIndexDistribution(randomGenerator)];
}

void Enemy::SetAnimSprite(std::unordered_map<std::string, std::vector<std::vector<std::string>>> newAnim)
{
    animSprite = newAnim;
}

std::vector<std::vector<std::string>>& Enemy::GetAnimSprite(const std::string& name)
{
    static std::vector<std::vector<std::string>> emptySprite;
    auto it = animSprite.find(name);
    if (it != animSprite.end())
    {
        return it->second;
    }
    return emptySprite;
}
