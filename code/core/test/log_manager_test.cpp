#include <gtest/gtest.h>

#include <iostream>
#include <filesystem>
#include <string>
#include "code/core/log_manager.h"

TEST(LogManager_Test, Create_LogManager) {
    const std::filesystem::path tmp_dir = std::getenv("TEST_TMPDIR"); // the bazel test file dir
    core::LogManager::get_Instance()->init(tmp_dir.string());

    core::LogManager::get_Instance()->shutdown();
}

TEST(LogManager_Test, Create_New_Logger) {
    const std::filesystem::path tmp_dir = std::getenv("TEST_TMPDIR"); // the bazel test file dir
    core::LogManager::get_Instance()->init(tmp_dir.string());

    CREATE_LOGGER(testCreate);

    LOG_TRACE(testCreate, "Trace logging")
    LOG_DEBUG(testCreate, "Debug logging")
    LOG_INFO(testCreate, "Info logging")
    LOG_WARN(testCreate, "Warn logging")
    LOG_ERROR(testCreate, "Error logging")
    LOG_CRITICAL(testCreate, "Critical logging")

    int numFiles = 0;
    EXPECT_TRUE(std::filesystem::is_directory(tmp_dir));
    for (const auto& entry : std::filesystem::directory_iterator(tmp_dir)) {
        std::filesystem::exists(entry.path());
        ++numFiles;
    }   
    EXPECT_GT(numFiles, 0);

    core::LogManager::get_Instance()->shutdown();
}

TEST(LogManager_Test, Create_New_Logger_Level) {
    const std::filesystem::path tmp_dir = std::getenv("TEST_TMPDIR"); // the bazel test file dir
    core::LogManager::get_Instance()->init(tmp_dir.string());

    CREATE_LOGGER_LEVEL(testTraceCreate, core::LogManager::LogLevel::TRACE);

    LOG_TRACE(testTraceCreate, "Trace logging")
    LOG_DEBUG(testTraceCreate, "Debug logging")
    LOG_INFO(testTraceCreate, "Info logging")
    LOG_WARN(testTraceCreate, "Warn logging")
    LOG_ERROR(testTraceCreate, std::string("Error logging ") + std::to_string(10))
    LOG_CRITICAL(testTraceCreate, "Critical logging")

    int numFiles = 0;
    EXPECT_TRUE(std::filesystem::is_directory(tmp_dir));
    for (const auto& entry : std::filesystem::directory_iterator(tmp_dir)) {
        std::filesystem::exists(entry.path());
        ++numFiles;
    }   
    EXPECT_GT(numFiles, 0);

    core::LogManager::get_Instance()->shutdown();
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
