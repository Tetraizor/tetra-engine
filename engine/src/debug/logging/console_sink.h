#pragma once

#include "Logger.h"
#ifdef _WIN32
#include <windows.h>
#endif

#include "debug/logging/logger.h"

class ConsoleSink : public LogSink
{
public:
    ConsoleSink()
    {
#ifdef _WIN32
        auto hOut = GetStdHandle(STD_OUTPUT_HANDLE);
        if (hOut != INVALID_HANDLE_VALUE)
        {
            DWORD mode = 0;
            if (GetConsoleMode(hOut, &mode))
                SetConsoleMode(hOut, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
        }
#endif
    }

    void write(const std::string &formatted_message, LogLevel level) override
    {
        const char *color_code = "\x1b[0m";
        switch (level)
        {
        case LogLevel::Debug:
            color_code = "\x1b[36m"; // cyan
            break;
        case LogLevel::Info:
            color_code = ""; // default
            break;
        case LogLevel::Warning:
            color_code = "\x1b[33m"; // yellow
            break;
        case LogLevel::Error:
            color_code = "\x1b[31m"; // red
            break;
        case LogLevel::Critical:
            color_code = "\x1b[41m"; // red bg
            break;
        }

        std::cout << color_code
                  << formatted_message
                  << "\x1b[0m"
                  << std::endl;
    }
};