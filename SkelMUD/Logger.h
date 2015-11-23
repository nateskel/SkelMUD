#ifndef LOGGER_H
#define LOGGER_H

#include <string>

class Logger {
public:
    enum Log_Level {
        L_INFO, L_ERROR, L_DEBUG, L_NONE
    };

    static void Info(std::string message);

    static void Error(std::string message);

    static void Debug(std::string message);

    static void SetOutputLevel(Log_Level level);

    static Log_Level m_level;

protected:
private:

    static std::string TimeStamp();

    static void logMessage(std::string log_type, std::string message);
};

#endif // LOGGER_H
