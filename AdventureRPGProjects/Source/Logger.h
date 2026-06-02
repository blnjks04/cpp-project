#pragma once
#include "Utils.h"
#include "Instance.h"

enum class LogType
{
	ALL,
	KeyEvent,
	BattleEvent,
	StateChange,
	Error
};
class Logger : public Instance<Logger>
{
public:

	Logger();
	~Logger() = default;

	std::string saveFileName;

	void AddLog(LogType type, const std::string& message);
	void SaveLogsToFile(LogType type, const std::string& message);
	
	// void ClearLogs();
	//std::vector<std::pair<LogType, std::string>> log;
    //void DisplayLogs(LogType filter = LogType::ALL);
};

#define LOG (Logger::GetInstance())
#define ADD_LOG(type, message) LOG->AddLog((type), (message))
