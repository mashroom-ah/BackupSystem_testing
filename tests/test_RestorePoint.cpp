#include <catch2/catch_test_macros.hpp>
#include "domain/RestorePoint.h"
#include <chrono>

TEST_CASE("RestorePoint full constructor", "[RestorePoint]") {
    auto now = std::chrono::system_clock::now();
    RestorePoint p("rp123", "/backup/point", now, BackupType::FULL);
    REQUIRE(p.getId() == "rp123");
    REQUIRE(p.getPath() == "/backup/point");
    REQUIRE(p.getTimestamp() == now);
    REQUIRE(p.getType() == BackupType::FULL);
}

TEST_CASE("RestorePoint incremental type", "[RestorePoint]") {
    auto now = std::chrono::system_clock::now();
    RestorePoint p("inc1", "/inc/path", now, BackupType::INCREMENTAL);
    REQUIRE(p.getType() == BackupType::INCREMENTAL);
}

TEST_CASE("RestorePoint default constructor creates empty", "[RestorePoint]") {
    RestorePoint p;
    REQUIRE(p.getId().empty());
    REQUIRE(p.getPath().empty());
}

TEST_CASE("RestorePoint timestamps can be compared", "[RestorePoint]") {
    auto t1 = std::chrono::system_clock::now();
    auto t2 = t1 + std::chrono::hours(1);
    RestorePoint p1("p1", "/p1", t1, BackupType::FULL);
    RestorePoint p2("p2", "/p2", t2, BackupType::INCREMENTAL);
    REQUIRE(p1.getTimestamp() < p2.getTimestamp());
}

TEST_CASE("RestorePoint copy semantics", "[RestorePoint]") {
    auto now = std::chrono::system_clock::now();
    RestorePoint p1("id", "/path", now, BackupType::FULL);
    RestorePoint p2 = p1;
    REQUIRE(p2.getId() == "id");
    REQUIRE(p2.getPath() == "/path");
    REQUIRE(p2.getTimestamp() == now);
    REQUIRE(p2.getType() == BackupType::FULL);
}