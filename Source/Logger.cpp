#include "Logger.h"
#include "Utils.h"
#include <fstream>
#include <chrono>
#include <locale>
#include <codecvt>

Logger::Logger()
{
	saveFileName = "game_log.txt";
}


void Logger::AddLog(LogType type, const std::string& message)
{
	SaveLogsToFile(type, message); 
}
void Logger::SaveLogsToFile(LogType type, const std::string& message)
{
	auto now = std::chrono::system_clock::now();
	auto currentTimeSeconds = std::chrono::floor<std::chrono::seconds>(now);

	std::ofstream outFile(saveFileName, std::ios::app);
	if (outFile.is_open())
	{
		outFile.imbue(std::locale("ko_KR.UTF-8")); 
		std::string logTypeText;
		switch (type)
		{
			case LogType::ALL: logTypeText = "[ALL] "; break;
			case LogType::KeyEvent: logTypeText = "[KeyEvent] "; break;
			case LogType::BattleEvent: logTypeText = "[BattleEvent] "; break;
			case LogType::StateChange: logTypeText = "[StateChange] "; break;
			case LogType::Error: logTypeText = "[Error] "; break;
			default: logTypeText = "ALL"; break;
		}
		outFile << std::format("{:%Y-%m-%d %H:%M:%S} : ", currentTimeSeconds) << logTypeText << message << "\n";
		outFile.close();
	}
	else
	{
		std::cerr << "Failed to open log file: " << saveFileName << std::endl;
	}
}
