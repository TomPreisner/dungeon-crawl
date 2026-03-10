#include <gtest/gtest.h>

#include <iostream>
#include <filesystem>
#include <string>
#include "code/core/log_manager.h"

TEST(LogManager_Test, Create_LogManager) {
    auto instance = core::LogManager::get_Instance();
    EXPECT_TRUE(instance != nullptr);
}

TEST(LogManager_Test, Create_New_Logger) {
    CREATE_LOGGER(testCreate);

    LOG_TRACE(testCreate, "Trace logging")
    LOG_DEBUG(testCreate, "Debug logging")
    LOG_INFO(testCreate, "Info logging")
    LOG_WARN(testCreate, "Warn logging")
    LOG_ERROR(testCreate, "Error logging")
    LOG_CRITICAL(testCreate, "Critical logging")

    int numFiles = 0;
    const std::filesystem::path tmp_dir = core::LogManager::get_Instance()->get_output_dir();
    EXPECT_TRUE(std::filesystem::is_directory(tmp_dir));
    for (const auto& entry : std::filesystem::directory_iterator(tmp_dir)) {
        std::filesystem::exists(entry.path());
        ++numFiles;
    }   
    EXPECT_GT(numFiles, 0);
}

TEST(LogManager_Test, Create_New_Logger_Level) {
    CREATE_LOGGER_LEVEL(testTraceCreate, core::LogManager::LogLevel::TRACE);

    LOG_TRACE(testTraceCreate, "Trace logging")
    LOG_DEBUG(testTraceCreate, "Debug logging")
    LOG_INFO(testTraceCreate, "Info logging")
    LOG_WARN(testTraceCreate, "Warn logging")
    LOG_ERROR(testTraceCreate, std::string("Error logging ") + std::to_string(10))
    LOG_CRITICAL(testTraceCreate, "Critical logging")

    int numFiles = 0;
    const std::filesystem::path tmp_dir = core::LogManager::get_Instance()->get_output_dir();
    EXPECT_TRUE(std::filesystem::is_directory(tmp_dir));
    for (const auto& entry : std::filesystem::directory_iterator(tmp_dir)) {
        std::filesystem::exists(entry.path());
        ++numFiles;
    }   
    EXPECT_GT(numFiles, 0);
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
