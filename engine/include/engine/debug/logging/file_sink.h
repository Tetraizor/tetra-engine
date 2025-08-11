#pragma once

#include "engine/debug/logging/logger.h"

class FileSink : public LogSink
{
public:
    explicit FileSink(const std::string &filename)
        : file_stream(filename, std::ios::out | std::ios::app) {}

    void write(const std::string &formatted_message, LogLevel level) override
    {
        if (file_stream.is_open())
        {
            file_stream << formatted_message << std::endl;
        }
    }

private:
    std::ofstream file_stream;
};