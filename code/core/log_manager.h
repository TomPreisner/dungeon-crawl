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
    enum class LogLevel {
        TRACE,
        DEBUG,
        INFO,
        WARN,
        ERR,    //< calling this ERROR causes compiler errors
        CRITICAL,
    };

    LogManager(LogManagerPasskey passkey) : LogManager() {        
        auto consoleSink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
        m_default_sinks.push_back(consoleSink);

        char * use_test_log_dir = std::getenv("USE_TEST_LOG_DIR");
        if (use_test_log_dir != nullptr) {
            if (std::stoi(use_test_log_dir) != 0) {
                char * test_tmpdir = std::getenv("TEST_TMPDIR");
                if (test_tmpdir != nullptr) {
                    m_output_dir = std::string(test_tmpdir);
                }
            }
        } else {
            char * log_dir = std::getenv("LOG_DIR");
            if (log_dir != nullptr) {
                m_output_dir = std::string(log_dir);
            }
        }

        if (!m_output_dir.empty()) {
            std::filesystem::path error_file_path = m_output_dir + "/error.log";
            auto errorLogSink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(error_file_path.string());
            errorLogSink->set_level(spdlog::level::warn);
            m_default_sinks.push_back(errorLogSink);
        }
    }

    ~LogManager() = default;

    std::vector<std::shared_ptr<spdlog::sinks::sink>> get_default_sinks() const {
        return m_default_sinks;
    }

    std::string get_output_dir() const {
        return m_output_dir;
    }

    static LogManager* get_Instance() {
        static LogManager::LogManagerPasskey s_passkey;
        static std::unique_ptr<LogManager> s_instance = std::make_unique<LogManager>(s_passkey);
        return s_instance.get();
    }

    static void init_logger(std::shared_ptr<spdlog::logger> logger, LogLevel log_level) {
        // set the default logging sinks
        logger->sinks().reserve(LogManager::get_Instance()->get_default_sinks().size());
        for (const auto& default_sink : LogManager::get_Instance()->get_default_sinks() ) {
            logger->sinks().push_back(default_sink);
        }

        spdlog::level::level_enum spdlog_level = spdlog::level::info;
        switch (log_level) {
            case LogLevel::TRACE:
                spdlog_level = spdlog::level::trace;
                break;
            case LogLevel::DEBUG:
                spdlog_level = spdlog::level::debug;
                break;
            case LogLevel::INFO:
                spdlog_level = spdlog::level::info;
                break;
            case LogLevel::WARN:
                spdlog_level = spdlog::level::warn;
                break;
            case LogLevel::ERR:
                spdlog_level = spdlog::level::err;
                break;
            case LogLevel::CRITICAL:
                spdlog_level = spdlog::level::critical;
                break;
        }

        // assign the log level
        logger->set_level(spdlog_level);
    }

private:
    LogManager() = default;
    LogManager(const LogManager&) = delete;
    LogManager& operator=(const LogManager&) = delete;

    std::vector<std::shared_ptr<spdlog::sinks::sink>> m_default_sinks;
    std::string m_output_dir;
};
} // namespace core 

#define CREATE_LOGGER_LEVEL(logName, logLevel) \
    class logName##_logger { \
     private: \
        std::shared_ptr<spdlog::logger> logName##_log; \
     public: \
        logName##_logger() { \
            logName##_log = std::make_shared<spdlog::logger>(std::string(#logName)); \
            ::core::LogManager::init_logger(logName##_log, logLevel); \
        } \
        std::shared_ptr<spdlog::logger> operator-> () { return logName##_log; } \
    } logName##_log;
    
#define CREATE_LOGGER(logName) \
    CREATE_LOGGER_LEVEL(logName, ::core::LogManager::LogLevel::INFO)

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
