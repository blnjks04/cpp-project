#include "EventDispatcher.h"

void EventDispatcher::Bind(const std::string& eventName, Character* instance, Delegate event)
{
	Listeners[eventName].push_back({instance, event});
}

void EventDispatcher::BroadCast(const std::string& eventName, const DamageEvent& event)
{
	if (DamageEvent* damageEvent = const_cast<DamageEvent*>(&event))
	{
		if (Listeners.find(eventName) != Listeners.end())
		{
			for (const auto& listener : Listeners[eventName])
			{
				listener.Listener(*damageEvent);
			}
		}
	}
}

void EventDispatcher::Unbind(const std::string& eventName, Character* instance)
{
	if (Listeners.find(eventName) != Listeners.end())
	{
		auto& listeners = Listeners[eventName];
		listeners.erase(std::remove_if(listeners.begin(), listeners.end(),
			[instance](const EventListener& listener)
			{
				return listener.ListenerInstance == instance;
			}), listeners.end());
	}
}