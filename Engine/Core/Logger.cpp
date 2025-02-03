

#include "Core/Logger.h"

#include <iostream>
#include <fstream>
#include <string>
#include <mutex>
#include <chrono>
#include <iomanip>
#include <sstream>

#include <format>


template <typename... Args>
void Logger::Log(LogLevel level, const std::string& format, Args&&... args)
{
    std::string RESETCOLOR = "\033[0m";
    std::lock_guard<std::mutex> lock(logMutex);
    std::string message = std::vformat(format, std::make_format_args(args...));

    std::string logEntry = "[" + GetTimestamp() + "] " + LevelToString(level) + " " + message;

    // Console Output with color reset
    std::cout << logEntry << RESETCOLOR << std::endl;

    // File Output (If Enabled)
    if (fileStream.is_open()) {
        fileStream << logEntry << std::endl;
    }
}
template void Logger::Log<>(LogLevel, const std::string&);
template void Logger::Log<char>(LogLevel, const std::string&, char&&);
template void Logger::Log<bool&>(LogLevel, const std::string&, bool&);
template void Logger::Log<const char*&>(LogLevel, const std::string&, const char*&);
template void Logger::Log<int&>(LogLevel, const std::string&, int&);
template void Logger::Log<int>(LogLevel, const std::string&, int&&);
template void Logger::Log<unsigned int>(LogLevel, const std::string&, unsigned int&&);
template void Logger::Log<unsigned int&>(LogLevel, const std::string&, unsigned int&);
template void Logger::Log<float>(LogLevel, const std::string&, float&&);
template void Logger::Log<double>(LogLevel, const std::string&, double&&);
template void Logger::Log<std::string>(LogLevel, const std::string&, std::string&&);

Logger& Logger::GetInstance()
{
    static Logger instance;
    return instance;
}

Logger::Logger()
{

}

Logger::~Logger()
{
    if (fileStream.is_open())
    {
        fileStream.close();
    }
}

void Logger::EnableFileLogging(const std::string& fileName)
{
    std::lock_guard<std::mutex> lock(logMutex);
    fileStream.open(fileName, std::ios::app);
    // if (!fileStream)
}

std::string Logger::LevelToString(LogLevel level) {
    switch (level) 
    {
        case LogLevel::FATAL: return "\033[97;41m[FATAL]";
        case LogLevel::ERROR: return "\033[31m[ERORR]";
        case LogLevel::WARNING: return "\033[33m[WARNING]";
        case LogLevel::INFO: return "\033[32m[INFO]";
        case LogLevel::DEBUG: return "\033[36m[DEBUG]";
        default: return "UNKNOWN";
    }
}

std::string Logger::GetTimestamp() {
    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);
    
    std::ostringstream ss;
    ss << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %H:%M:%S");
    return ss.str();
}

