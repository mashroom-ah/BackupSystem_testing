#include <catch2/catch_test_macros.hpp>
#include "application/BackupService.h"
#include <fstream>
#include <filesystem>

TEST_CASE("BackupService first backup creates FULL point", "[BackupService]") {
    auto src = std::filesystem::temp_directory_path() / "bs_src";
    auto dst = std::filesystem::temp_directory_path() / "bs_dst";
    std::filesystem::create_directories(src);
    std::ofstream(src / "f.txt") << "data";
    Schedule s; RetentionPolicy r;
    BackupJob job("j1", "test", src, dst, s, r);
    BackupService bs;
    bs.runBackup(job);
    REQUIRE(job.getRestorePoints().size() == 1);
    REQUIRE(job.getRestorePoints()[0].getType() == BackupType::FULL);
    std::filesystem::remove_all(src);
    std::filesystem::remove_all(dst);
}

TEST_CASE("BackupService incremental after file change", "[BackupService]") {
    auto src = std::filesystem::temp_directory_path() / "inc_src";
    auto dst = std::filesystem::temp_directory_path() / "inc_dst";
    std::filesystem::create_directories(src);
    std::ofstream(src / "f.txt") << "v1";
    Schedule s; RetentionPolicy r;
    BackupJob job("j2", "test", src, dst, s, r);
    BackupService bs;
    bs.runBackup(job);
    std::ofstream(src / "f.txt", std::ios::trunc) << "v2";
    bs.runBackup(job);
    REQUIRE(job.getRestorePoints().size() == 2);
    REQUIRE(job.getRestorePoints()[1].getType() == BackupType::INCREMENTAL);
    std::filesystem::remove_all(src);
    std::filesystem::remove_all(dst);
}

TEST_CASE("BackupService no changes still creates a point (empty)", "[BackupService]") {
    auto src = std::filesystem::temp_directory_path() / "nochange_src";
    auto dst = std::filesystem::temp_directory_path() / "nochange_dst";
    std::filesystem::create_directories(src);
    std::ofstream(src / "f.txt") << "same";
    Schedule s; RetentionPolicy r;
    BackupJob job("j3", "test", src, dst, s, r);
    BackupService bs;
    bs.runBackup(job);
    bs.runBackup(job);
    REQUIRE(job.getRestorePoints().size() == 2);
    std::filesystem::remove_all(src);
    std::filesystem::remove_all(dst);
}

TEST_CASE("BackupService multiple files incremental", "[BackupService]") {
    auto src = std::filesystem::temp_directory_path() / "multi_src";
    auto dst = std::filesystem::temp_directory_path() / "multi_dst";
    std::filesystem::create_directories(src);
    std::ofstream(src / "a.txt") << "a1";
    std::ofstream(src / "b.txt") << "b1";
    Schedule s; RetentionPolicy r;
    BackupJob job("j4", "test", src, dst, s, r);
    BackupService bs;
    bs.runBackup(job);
    std::ofstream(src / "a.txt", std::ios::trunc) << "a2";
    bs.runBackup(job);
    REQUIRE(job.getRestorePoints().size() == 2);
    REQUIRE(job.getRestorePoints()[1].getType() == BackupType::INCREMENTAL);
    std::filesystem::remove_all(src);
    std::filesystem::remove_all(dst);
}

TEST_CASE("BackupService handles empty source directory", "[BackupService]") {
    auto src = std::filesystem::temp_directory_path() / "empty_src";
    auto dst = std::filesystem::temp_directory_path() / "empty_dst";
    std::filesystem::create_directories(src);
    Schedule s; RetentionPolicy r;
    BackupJob job("j5", "test", src, dst, s, r);
    BackupService bs;
    REQUIRE_NOTHROW(bs.runBackup(job));
    REQUIRE(job.getRestorePoints().size() == 1);
    std::filesystem::remove_all(src);
    std::filesystem::remove_all(dst);
}