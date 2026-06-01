#pragma once
#include "Component.h"
#include "Utils.h"

// 캐릭터 Entity

class Character
{
private:
	std::string name;
	std::vector<std::unique_ptr<Component>> components;
public:
    Character(const std::string& name) : name(name) {};
	~Character() = default;
	std::string GetName() const;
	virtual void TakeDamage(int damage) = 0;
	void AddComponent(std::unique_ptr<Component> component);

	template<typename T>
	T* GetComponent()
	{
		for (const auto& component : components)
		{
			T* castedComponent = dynamic_cast<T*>(component.get());
			if (castedComponent)
				return castedComponent;
		}
		return nullptr;
	}
};
