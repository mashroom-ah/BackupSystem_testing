#include <catch2/catch_test_macros.hpp>
#include "domain/Schedule.h"
#include <thread>

// Конструкторы
TEST_CASE("Schedule default constructor", "[Schedule]") {
    Schedule s;
    REQUIRE(s.getInterval() == 0);
    REQUIRE_FALSE(s.isEnabled());
}

TEST_CASE("Schedule parameterized constructor with zero interval", "[Schedule]") {
    Schedule s(0, true);
    REQUIRE(s.getInterval() == 0);
    REQUIRE(s.isEnabled());
}

TEST_CASE("Schedule parameterized with large interval", "[Schedule]") {
    Schedule s(86400, false);
    REQUIRE(s.getInterval() == 86400);
    REQUIRE_FALSE(s.isEnabled());
}

// setEnabled
TEST_CASE("Schedule setEnabled toggles state", "[Schedule]") {
    Schedule s(10, true);
    s.setEnabled(false);
    REQUIRE_FALSE(s.isEnabled());
    s.setEnabled(true);
    REQUIRE(s.isEnabled());
    s.setEnabled(false);
    REQUIRE_FALSE(s.isEnabled());
}

// updateLastRun и getLastRun
TEST_CASE("Schedule updateLastRun stores time", "[Schedule]") {
    Schedule s;
    auto t1 = std::chrono::system_clock::now();
    s.updateLastRun(t1);
    REQUIRE(s.getLastRun() == t1);
    auto t2 = t1 + std::chrono::hours(1);
    s.updateLastRun(t2);
    REQUIRE(s.getLastRun() == t2);
}

TEST_CASE("Schedule shouldRun with disabled always false", "[Schedule]") {
    Schedule s(5, false);
    auto now = std::chrono::system_clock::now();
    REQUIRE_FALSE(s.shouldRun(now));
    s.updateLastRun(now - std::chrono::seconds(100));
    REQUIRE_FALSE(s.shouldRun(now));
}

TEST_CASE("Schedule shouldRun respects interval", "[Schedule]") {
    Schedule s(10, true);
    auto start = std::chrono::system_clock::now();
    s.updateLastRun(start);
    REQUIRE_FALSE(s.shouldRun(start + std::chrono::seconds(5)));
    REQUIRE(s.shouldRun(start + std::chrono::seconds(10)));
    REQUIRE(s.shouldRun(start + std::chrono::seconds(15)));
}

TEST_CASE("Schedule shouldRun with zero interval always true", "[Schedule]") {
    Schedule s(0, true);
    auto now = std::chrono::system_clock::now();
    s.updateLastRun(now);
    REQUIRE(s.shouldRun(now));
    REQUIRE(s.shouldRun(now + std::chrono::seconds(1)));
}

TEST_CASE("Schedule shouldRun after update resets timer", "[Schedule]") {
    Schedule s(5, true);
    auto start = std::chrono::system_clock::now();
    s.updateLastRun(start);
    REQUIRE_FALSE(s.shouldRun(start + std::chrono::seconds(3)));
    s.updateLastRun(start + std::chrono::seconds(3));
    REQUIRE_FALSE(s.shouldRun(start + std::chrono::seconds(6))); // прошло 3 сек после обновления
    REQUIRE(s.shouldRun(start + std::chrono::seconds(9)));       // 6 сек после обновления
}