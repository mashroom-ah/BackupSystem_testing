#include <catch2/catch_test_macros.hpp>
#include "domain/RetentionPolicy.h"

TEST_CASE("RetentionPolicy defaults", "[RetentionPolicy]") {
    RetentionPolicy r;
    REQUIRE(r.getMaxStorageSize() == 0);
    REQUIRE(r.getMaxDays() == 0);
    REQUIRE(r.getMinRestorePoints() == 3);
}

TEST_CASE("RetentionPolicy changeStorageSize", "[RetentionPolicy]") {
    RetentionPolicy r;
    r.changeStorageSize(1024);
    REQUIRE(r.getMaxStorageSize() == 1024);
    r.changeStorageSize(0);
    REQUIRE(r.getMaxStorageSize() == 0);
    r.changeStorageSize(-100);
    REQUIRE(r.getMaxStorageSize() == -100);
}

TEST_CASE("RetentionPolicy changeMaxDays", "[RetentionPolicy]") {
    RetentionPolicy r;
    r.changeMaxDays(30);
    REQUIRE(r.getMaxDays() == 30);
    r.changeMaxDays(0);
    REQUIRE(r.getMaxDays() == 0);
    r.changeMaxDays(-5);
    REQUIRE(r.getMaxDays() == -5);
}

TEST_CASE("RetentionPolicy changeMinRestorePoints", "[RetentionPolicy]") {
    RetentionPolicy r;
    r.changeMinRestorePoints(5);
    REQUIRE(r.getMinRestorePoints() == 5);
    r.changeMinRestorePoints(1);
    REQUIRE(r.getMinRestorePoints() == 1);
    r.changeMinRestorePoints(0);
    REQUIRE(r.getMinRestorePoints() == 0);
}

TEST_CASE("RetentionPolicy multiple changes", "[RetentionPolicy]") {
    RetentionPolicy r;
    r.changeStorageSize(500);
    r.changeMaxDays(7);
    r.changeMinRestorePoints(2);
    REQUIRE(r.getMaxStorageSize() == 500);
    REQUIRE(r.getMaxDays() == 7);
    REQUIRE(r.getMinRestorePoints() == 2);
}

TEST_CASE("RetentionPolicy independent modifications", "[RetentionPolicy]") {
    RetentionPolicy r1, r2;
    r1.changeStorageSize(100);
    r2.changeStorageSize(200);
    REQUIRE(r1.getMaxStorageSize() == 100);
    REQUIRE(r2.getMaxStorageSize() == 200);
}