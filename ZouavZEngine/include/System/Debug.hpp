#pragma once
#include <string>
#include <unordered_map>
#include <iostream>

#define ZASSERT(condition, message) if (!(condition)) { std::cerr << "Assertion failed : " << message << "\nAt " << __FUNCTION__ << "\nIn " << __FILE__ << "\nLine " << __LINE__; abort(); }

enum class E_LOGS_TYPE
{
	TEXT,
	WARNING,
	ERROR
};

class Debug
{
private:
public:
	Debug() = delete;
	~Debug() = delete;
	
	static std::unordered_map<std::string, E_LOGS_TYPE> logs;

	static void Log(std::string log);
	static void LogWarning(std::string warningLog);
	static void LogError(std::string errorLog);
	//stop play
	//static void Assert(bool condition);
};