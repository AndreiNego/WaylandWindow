

#ifndef CORE_H
#define CORE_H

// Logging settings
// For additional platform support, this must be modified
#include "Core/Logger.h"

///Usage: "Message + {} for each additional parameter ", param1, param2) Core Logging supports simple formats: int, float, string
#define AND_FATAL(format, ...) Logger::GetInstance().Log(LogLevel::FATAL, format, ##__VA_ARGS__)
#define AND_ERROR(format, ...) Logger::GetInstance().Log(LogLevel::ERROR, format, ##__VA_ARGS__)
#define AND_WARNING(format, ...) Logger::GetInstance().Log(LogLevel::WARNING, format, ##__VA_ARGS__)
#define AND_INFO(format, ...) Logger::GetInstance().Log(LogLevel::INFO, format, ##__VA_ARGS__)

#ifdef _DEBUG
#define AND_DEBUG(format, ...) Logger::GetInstance().Log(LogLevel::DEBUG, format, ##__VA_ARGS__)
#else
#define AND_DEBUG(format, ...) ((void)0);
#endif

#endif // !CORE_H

