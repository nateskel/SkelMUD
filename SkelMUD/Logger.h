#ifndef LOGGER_H
#define LOGGER_H

#include <string>

class Logger {
public:
    enum Log_Level {
        L_INFO, L_ERROR, L_DEBUG, L_NONE
    };

    // Logger(Log_Level level);

    static void Info(std::string message);

    static void Error(std::string message);

    static void Debug(std::string message);

    static void SetOutputLevel(Log_Level level);

    static Log_Level m_level;

protected:
private:

    static std::string TimeStamp();
};

#endif // LOGGER_H
