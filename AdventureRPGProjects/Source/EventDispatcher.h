#pragma once
#include "Utils.h"
#include "Instance.h"
#include <functional>

class Character;

struct DamageEvent
{
	Character* Attacker;
	Character* target;
	int damage;
};

using Delegate = std::function<void(const DamageEvent)>;

struct EventListener
{
	Character* ListenerInstance;
	Delegate Listener;
};

class EventDispatcher : public Instance<EventDispatcher>
{
	friend class Instance<EventDispatcher>;
private:
	std::map<std::string, std::vector<EventListener>> Listeners;

public:
	void Bind(const std::string& eventName, Character* instance, Delegate event);
	void Unbind(const std::string& eventName, Character* instance);
	void BroadCast(const std::string& eventName, const DamageEvent& event);
};
