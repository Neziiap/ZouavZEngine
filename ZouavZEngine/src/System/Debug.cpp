#include "System/Debug.hpp"

std::unordered_map<std::string, E_LOGS_TYPE> Debug::logs;

void Debug::Log(std::string log)
{
	logs.emplace(log, E_LOGS_TYPE::TEXT);
}

void Debug::LogWarning(std::string warningLog)
{
	logs.emplace(warningLog, E_LOGS_TYPE::WARNING);
}

void Debug::LogError(std::string errorLog)
{
	logs.emplace(errorLog, E_LOGS_TYPE::ERROR);
}