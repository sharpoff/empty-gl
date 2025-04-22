#pragma once

#include <iostream>
#include <string_view>
#include <cstdlib>
#include <string>

enum LogType {
    LOG_WARNING,
    LOG_ERROR,
    LOG_INFO
};

enum LogColor {
    YELLOW = 33,
    RED = 31,
    WHITE = 39,
    GRAY = 90
};

class Logger
{
public:
    static void print(std::string_view message, LogType type);
    static void saveLogToFile(std::string_view filename);
private:
    //static std::vector<std::vector<std::string>> mLogMessages;
};
