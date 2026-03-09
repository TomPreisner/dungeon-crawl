/********************************************************************/
/*                      Copyright 2026                              */
/*           Created and Maintained by Thomas Preisner              */
/********************************************************************/
#pragma once

#include <filesystem>
#include <memory>
#include <string>
#include <vector>

#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h> 
#include <spdlog/sinks/sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace core {
class LogManager {
private:
    // The passkey pattern is necessary here to use the the unique_ptr without
    //  exposing a constructor that other classes can use to create an instance
    class LogManagerPasskey {
     private:
        friend LogManager;
        LogManagerPasskey() = default;
        ~LogManagerPasskey() = default;
    };
public:
    LogManager(LogManagerPasskey passkey) : LogManager() {}
    ~LogManager() = default;

    void init(const std::string& log_dir) {
        if (s_initialized) {
            return; //<  only initialize once
        }
        s_initialized = true;

        std::filesystem::path error_file_path = log_dir + "/error.log";
        auto consoleSink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
        auto errorLogSink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(error_file_path.string());
        errorLogSink->set_level(spdlog::level::warn);

        m_default_sinks.reserve(3);
        m_default_sinks.push_back(consoleSink);
        m_default_sinks.push_back(errorLogSink);
    }

    void shutdown() {
        m_default_sinks.clear();
        s_initialized = false;        
    }

    std::vector<std::shared_ptr<spdlog::sinks::sink>> get_default_sinks() const {
        return m_default_sinks;
    }

    static LogManager* get_Instance() {
        static LogManager::LogManagerPasskey s_passkey;
        static std::unique_ptr<LogManager> s_instance = std::make_unique<LogManager>(s_passkey);
        return s_instance.get();
    }

    static void init_logger(std::shared_ptr<spdlog::logger> logger, spdlog::level::level_enum log_level) {
        // set the default logging sinks
        logger->sinks().reserve(LogManager::get_Instance()->get_default_sinks().size());
        for (const auto& default_sink : LogManager::get_Instance()->get_default_sinks() ) {
            logger->sinks().push_back(default_sink);
        }

        // assign the log level
        logger->set_level(log_level);
    }

private:
    LogManager() = default;
    LogManager(const LogManager&) = delete;
    LogManager& operator=(const LogManager&) = delete;

    static inline bool s_initialized = false;
    std::vector<std::shared_ptr<spdlog::sinks::sink>> m_default_sinks;
};
} // namespace core 

#define CREATE_LOGGER_LEVEL(logName, logLevel) \
    std::shared_ptr<spdlog::logger> logName##_log = std::make_shared<spdlog::logger>(std::string(#logName)); \
    core::LogManager::init_logger(logName##_log, logLevel);
    
#define CREATE_LOGGER(logName) \
    CREATE_LOGGER_LEVEL(logName, spdlog::level::info)

#define LOG_TRACE(logName, entry) \
    logName##_log->log(spdlog::level::trace, entry);

#define LOG_DEBUG(logName, entry) \
    logName##_log->log(spdlog::level::debug, entry);

#define LOG_INFO(logName, entry) \
    logName##_log->log(spdlog::level::info, entry);

#define LOG_WARN(logName, entry) \
    logName##_log->log(spdlog::level::warn, entry);

#define LOG_ERROR(logName, entry) \
    logName##_log->log(spdlog::level::err, entry);

#define LOG_CRITICAL(logName, entry) \
    logName##_log->log(spdlog::level::critical, entry);
