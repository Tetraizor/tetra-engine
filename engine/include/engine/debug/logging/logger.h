#pragma once

#include <string>
#include <memory>
#include <vector>
#include <mutex>
#include <fstream>
#include <sstream>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <thread>
#include <iostream>

#include "engine/base/singleton.h"

// Severity levels for log messages
enum class LogLevel
{
    Debug,   // Detailed diagnostic information
    Info,    // General operational messages
    Warning, // Indication of potential issues
    Error,   // Errors that allow continuation
    Critical // Severe errors causing abort
};

inline const char *log_level_to_string(LogLevel level)
{
    switch (level)
    {
    case LogLevel::Debug:
        return "DEBUG";
    case LogLevel::Info:
        return "INFO";
    case LogLevel::Warning:
        return "WARNING";
    case LogLevel::Error:
        return "ERROR";
    case LogLevel::Critical:
        return "CRITICAL";
    default:
        return "UNKNOWN";
    }
}

// Abstract interface for log output sinks (console, file, network, etc.)
class LogSink
{
public:
    virtual ~LogSink() = default;
    virtual void write(const std::string &formatted_message, LogLevel level) = 0;
};

class Logger : public Engine::Singleton<Logger>
{
public:
    static void log(const std::string &message, LogLevel level = LogLevel::Info)
    {
        get_instance().log_instance(message, level);
    }

    static void log_info(const std::string &message)
    {
        get_instance().log_instance(message, LogLevel::Info);
    }

    static void log_debug(const std::string &message)
    {
        get_instance().log_instance(message, LogLevel::Debug);
    }
    static void log_warning(const std::string &message)
    {
        get_instance().log_instance(message, LogLevel::Warning);
    }
    static void log_error(const std::string &message)
    {
        get_instance().log_instance(message, LogLevel::Error);
    }
    static void log_critical(const std::string &message)
    {
        get_instance().log_instance(message, LogLevel::Critical);
    }

    // Log a message with level, category, and text
    void log_instance(const std::string &message, LogLevel level = LogLevel::Debug)
    {
        if (level < min_level)
            return;

        auto now = std::chrono::system_clock::now();
        auto time_t_now = std::chrono::system_clock::to_time_t(now);
        std::stringstream time_ss;
        time_ss << std::put_time(std::localtime(&time_t_now), "%Y-%m-%d %H:%M:%S");

        // Format final message
        std::stringstream ss;
        ss << "[" + time_ss.str() + "] " << message;

        std::lock_guard<std::mutex> lock(mutex);
        for (auto &sink : sinks)
        {
            sink->write(ss.str(), level);
        }
    }

    void set_min_level(LogLevel level)
    {
        min_level = level;
    }

    void add_sink(std::shared_ptr<LogSink> sink)
    {
        std::lock_guard<std::mutex> lock(mutex);
        sinks.push_back(std::move(sink));
    }

private:
    LogLevel min_level = LogLevel::Debug;        // Default: log all levels
    std::vector<std::shared_ptr<LogSink>> sinks; // All registered sinks
    std::mutex mutex;                            // Guards sinks and writes
};

class LogBuilder
{
public:
    LogBuilder(LogLevel level)
        : level(level) {}

    ~LogBuilder()
    {
        Logger::get_instance().log_instance(stream.str(), level);
    }

    template <typename T>
    LogBuilder &operator<<(const T &value)
    {
        stream << value;
        return *this;
    }

private:
    LogLevel level;
    std::ostringstream stream;
};

#define LOG_DEBUG(category, message) Logger::get_instance().log_instance(message, LogLevel::Debug)
#define LOG_INFO(category, message) Logger::get_instance().log_instance(message, LogLevel::Info)
#define LOG_WARNING(category, message) Logger::get_instance().log_instance(message, LogLevel::Warning)
#define LOG_ERROR(category, message) Logger::get_instance().log_instance(message, LogLevel::Error)
#define LOG_CRITICAL(category, message) Logger::get_instance().log_instance(message, LogLevel::Critical)