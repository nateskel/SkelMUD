#include "Logger.h"
#include "Utils.h"
#include <iostream>
#include <time.h>

Logger::Logger(Log_Level level)
{
	m_level = level;
}

void Logger::Debug(std::string message)
{
	if (m_level == L_DEBUG)
	{
		std::cout << TimeStamp() << " DEBUG: " << " " << message << std::endl;
	}
}

void Logger::Info(std::string message)
{
	if (m_level == L_DEBUG || m_level == L_INFO)
	{
		std::cout << TimeStamp() << " INFO: " << " " << message << std::endl;
	}
}

void Logger::Error(std::string message)
{
	if (m_level == L_DEBUG || m_level == L_INFO || m_level == L_ERROR)
	{
		std::cout << TimeStamp() << " ERROR: " << " " << message << std::endl;
	}
}

std::string Logger::TimeStamp()
{
	time_t rawtime;
	struct tm timeinfo;

	time(&rawtime);
	localtime_s(&timeinfo, &rawtime);
	char timebuf[26];
	asctime_s(timebuf, 26, &timeinfo);
	std::string output = std::string(timebuf);
	Utils::RemoveEndline(output);
	return output;
}