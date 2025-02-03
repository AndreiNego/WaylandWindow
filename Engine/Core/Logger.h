
#ifndef LOGGER_H
#define LOGGER_H

#include <fstream>
#include <string>
#include <mutex>


enum class LogLevel { FATAL, ERROR, WARNING, INFO, DEBUG };

class Logger
{
public:
    static Logger& GetInstance();
    void EnableFileLogging(const std::string& filename);
    
    template <typename... Args>
    void Log(LogLevel level, const std::string& format, Args&&... args);

private:
    Logger();
    ~Logger();

    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    std::string GetTimestamp();
    std::string LevelToString(LogLevel level);

    std::ofstream fileStream;
    std::mutex logMutex;
};

#endif //LOGGER_H
