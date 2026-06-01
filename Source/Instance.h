#pragma once
#include "Utils.h"

template<typename T>
class Instance 
{
protected:
	Instance() = default;
	virtual ~Instance() = default;
private:	
	static T* instance;
public:
	
	Instance(const Instance&) = delete;
	Instance& operator=(const Instance&) = delete;
	static T* GetInstance()
	{
		if (instance == nullptr)
		{
			instance = new T();
		}
		return instance;
	}

};

template<typename T>
T* Instance<T>::instance = nullptr;