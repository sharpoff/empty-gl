#include "logger.h"

void Logger::print(std::string_view message, LogType type)
{
    LogColor color = GRAY;
    std::string typeName;

    if (type == LOG_WARNING) {
        color = YELLOW;
        typeName = "WARNING";
    } else if (type == LOG_ERROR) {
        color = RED;
        typeName = "ERROR";
    } else if (type == LOG_INFO) {
        color = GRAY;
        typeName = "INFO";
    }

    std::cout << "\033[" << color << "m" << "[" << typeName << "] " << __TIME__ << ": " << message << "\033[m" << std::endl;
}

void Logger::saveLogToFile(std::string_view filename)
{
    // TODO: save all log messages to file
}
