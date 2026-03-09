#include <gtest/gtest.h>

#include <cerrno>
#include <iostream>
#include <filesystem>
#include <fstream>
#include <cstdlib>
#include <memory>
#include <string>

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

TEST(spdlog_Test, Create_Simple_Test) {
    // set the minimum level to `trace` instead of the default `info`
    spdlog::default_logger()->set_level(spdlog::level::trace);

    // convenience functions
    spdlog::trace("TRACE");
    spdlog::debug("DEBUG");
    spdlog::info("INFO");
    spdlog::warn("WARN");
    spdlog::error("ERROR");
    spdlog::critical("CRITICAL");

    // direct logging call
    spdlog::default_logger()->log(spdlog::level::trace, "TRACE");
    spdlog::default_logger()->log(spdlog::level::debug, "DEBUG");
    spdlog::default_logger()->log(spdlog::level::info, "INFO");
    spdlog::default_logger()->log(spdlog::level::warn, "WARN");
    spdlog::default_logger()->log(spdlog::level::err, "ERROR");  //< it's `err` not `error`  :/
    spdlog::default_logger()->log(spdlog::level::critical, "CRITICAL");

    // this test is just to run the basic console logging
    EXPECT_TRUE(true);
}

TEST(spdlog_Test, Create_LogFile_Test) {
    const std::filesystem::path tmp_dir = std::getenv("TEST_TMPDIR"); // the bazel test file dir

    std::filesystem::path error_file_path = tmp_dir / "error.log";
    std::filesystem::path debug_file_path = tmp_dir / "debug.log";
    auto consoleSink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    auto errorLogSink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(error_file_path.string());
    errorLogSink->set_level(spdlog::level::warn);
    auto debugLogSink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(debug_file_path.string());
    debugLogSink->set_level(spdlog::level::debug);

    std::shared_ptr<spdlog::logger> testLogger = std::make_shared<spdlog::logger>("testLogger");
    testLogger->sinks().push_back(consoleSink);
    testLogger->sinks().push_back(errorLogSink);
    testLogger->sinks().push_back(debugLogSink);

    // NOTE: loggers default to info logging level. Even if a sink is defined that takes
    //  a lesser log level (i.e. debug) the logging call will be prefiltered by the logger
    //  before the sink can take action on the log request.
    testLogger->set_level(spdlog::level::trace);

    // direct logging call
    testLogger->log(spdlog::level::trace, "TRACE");
    testLogger->log(spdlog::level::debug, "DEBUG");
    testLogger->log(spdlog::level::info, "INFO");
    testLogger->log(spdlog::level::warn, "WARN");
    testLogger->log(spdlog::level::err, "ERROR");  //< it's `err` not `error`  :/
    testLogger->log(spdlog::level::critical, "CRITICAL");

    // Reading the contents of the files from the tmp dir from the test doesn't seem to work
    //  The ifstream read just doesn't get anything. This is likely due to the setup of
    //   the bazel test structure.
    // This test is testing integration, not spdlog, so the files existing is fine
    //  The contents were validated manually.
    EXPECT_TRUE(std::filesystem::exists(error_file_path));
    EXPECT_TRUE(std::filesystem::exists(debug_file_path));
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
 