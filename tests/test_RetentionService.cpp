#include <catch2/catch_test_macros.hpp>
#include "infrastructure/RetentionService.h"
#include <fstream>
#include <filesystem>
#include <chrono>

static void createFakeRestorePoint(BackupJob& job, const std::string& id, int ageHours, size_t sizeKB = 1) {
    auto time = std::chrono::system_clock::now() - std::chrono::hours(ageHours);
    auto path = std::filesystem::temp_directory_path() / ("rp_" + id);
    std::ofstream ofs(path, std::ios::binary);
    ofs << std::string(sizeKB * 1024, 'X');
    RestorePoint point(id, path, time, BackupType::FULL);
    job.addRestorePoint(point);
}

TEST_CASE("RetentionService keeps at least minRestorePoints (default 3)", "[Retention]") {
    BackupJob job("id", "name", "/s", "/d", Schedule(), RetentionPolicy());
    for (int i = 0; i < 5; ++i) {
        createFakeRestorePoint(job, "p" + std::to_string(i), i, 1);
    }
    RetentionService rs;
    rs.applyRetention(job);
    REQUIRE(job.getRestorePoints().size() == 3);
    for (auto& p : job.getRestorePoints()) {
        std::filesystem::remove(p.getPath());
    }
}

TEST_CASE("RetentionService respects custom minRestorePoints", "[Retention]") {
    BackupJob job("id", "name", "/s", "/d", Schedule(), RetentionPolicy());
    job.getRetention().changeMinRestorePoints(1);
    for (int i = 0; i < 5; ++i) {
        createFakeRestorePoint(job, "p" + std::to_string(i), i, 1);
    }
    RetentionService rs;
    rs.applyRetention(job);
    REQUIRE(job.getRestorePoints().size() == 1);
    for (auto& p : job.getRestorePoints()) {
        std::filesystem::remove(p.getPath());
    }
}

TEST_CASE("RetentionService removes points older than maxDays", "[Retention]") {
    BackupJob job("id", "name", "/s", "/d", Schedule(), RetentionPolicy());
    job.getRetention().changeMaxDays(2);
    createFakeRestorePoint(job, "old", 72, 1);   // 3 дня
    createFakeRestorePoint(job, "new", 24, 1);   // 1 день
    RetentionService rs;
    rs.applyRetention(job);
    REQUIRE(job.getRestorePoints().size() == 1);
    REQUIRE(job.getRestorePoints()[0].getId() == "new");
    std::filesystem::remove(job.getRestorePoints()[0].getPath());
}

TEST_CASE("RetentionService respects maxStorageSize", "[Retention]") {
    BackupJob job("id", "name", "/s", "/d", Schedule(), RetentionPolicy());
    job.getRetention().changeStorageSize(1); // 1 MB limit
    createFakeRestorePoint(job, "p1", 0, 600); // 0.6 MB
    createFakeRestorePoint(job, "p2", 1, 600); // 0.6 MB
    RetentionService rs;
    rs.applyRetention(job);
    // total 1.2 MB, should remove oldest (p1)
    REQUIRE(job.getRestorePoints().size() == 1);
    REQUIRE(job.getRestorePoints()[0].getId() == "p2");
    std::filesystem::remove(job.getRestorePoints()[0].getPath());
}

TEST_CASE("RetentionService maxStorageSize does not remove below min points", "[Retention]") {
    BackupJob job("id", "name", "/s", "/d", Schedule(), RetentionPolicy());
    job.getRetention().changeStorageSize(1);
    job.getRetention().changeMinRestorePoints(3);
    for (int i = 0; i < 3; ++i) {
        createFakeRestorePoint(job, "p" + std::to_string(i), i, 500); // 0.5 MB each
    }
    RetentionService rs;
    rs.applyRetention(job);
    REQUIRE(job.getRestorePoints().size() == 3);
    for (auto& p : job.getRestorePoints()) {
        std::filesystem::remove(p.getPath());
    }
}

TEST_CASE("RetentionService combines maxDays and maxStorageSize", "[Retention]") {
    BackupJob job("id", "name", "/s", "/d", Schedule(), RetentionPolicy());
    job.getRetention().changeMaxDays(1);
    job.getRetention().changeStorageSize(1); // 1 MB
    createFakeRestorePoint(job, "very_old", 48, 500);
    createFakeRestorePoint(job, "old", 25, 600);
    createFakeRestorePoint(job, "new", 10, 400);
    RetentionService rs;
    rs.applyRetention(job);
    REQUIRE(job.getRestorePoints().size() == 1);
    REQUIRE(job.getRestorePoints()[0].getId() == "new");
    std::filesystem::remove(job.getRestorePoints()[0].getPath());
}

TEST_CASE("RetentionService handles empty points list", "[Retention]") {
    BackupJob job("id", "name", "/s", "/d", Schedule(), RetentionPolicy());
    RetentionService rs;
    REQUIRE_NOTHROW(rs.applyRetention(job));
    REQUIRE(job.getRestorePoints().empty());
}