#ifndef LOGGER_H
#define LOGGER_H

#include <string>

class Logger {
public:
    enum Log_Level {
        L_INFO, L_ERROR, L_DEBUG, L_NONE
    };

    Logger(Log_Level level);

    void Info(std::string message);

    void Error(std::string message);

    void Debug(std::string message);

    void SetOutputLevel(Log_Level level);

protected:
private:
    Log_Level m_level;

    std::string TimeStamp();
};

#endif // LOGGER_H
