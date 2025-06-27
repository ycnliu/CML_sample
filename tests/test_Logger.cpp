#include "catch.hpp"
#include "Logger.h"

TEST_CASE("Logger stores and clears messages", "[Logger]") {
    Logger log;
    log.clear();
    log.log("Test message 1");
    log.log("Test message 2");
    const auto& messages = log.getLogs();
    REQUIRE(messages.size() == 2);
    REQUIRE(messages[0] == std::string("Test message 1"));
    REQUIRE(messages[1] == std::string("Test message 2"));
    // Clear logs and verify
    log.clear();
    REQUIRE(log.getLogs().empty());
}

TEST_CASE("Multiple Logger instances share log storage", "[Logger]") {
    Logger log1;
    Logger log2;
    log1.clear();
    log1.log("Entry A");
    log2.log("Entry B");
    // Both logger instances should see the combined log messages
    const auto& msgs1 = log1.getLogs();
    const auto& msgs2 = log2.getLogs();
    REQUIRE(msgs1.size() == 2);
    REQUIRE(msgs2.size() == 2);
    REQUIRE(std::find(msgs1.begin(), msgs1.end(), std::string("Entry A")) != msgs1.end());
    REQUIRE(std::find(msgs1.begin(), msgs1.end(), std::string("Entry B")) != msgs1.end());
}
