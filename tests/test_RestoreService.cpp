#include <catch2/catch_test_macros.hpp>
#include "application/RestoreService.h"
#include "application/BackupService.h"
#include <fstream>
#include <filesystem>

TEST_CASE("RestoreService restores a full backup point", "[RestoreService]") {
    auto src = std::filesystem::temp_directory_path() / "rest_src";
    auto dst = std::filesystem::temp_directory_path() / "rest_dst";
    std::filesystem::create_directories(src);
    std::ofstream(src / "file.txt") << "restore me";
    Schedule s; RetentionPolicy r;
    BackupJob job("j", "test", src, dst, s, r);
    BackupService bs;
    bs.runBackup(job);
    auto pointId = job.getRestorePoints()[0].getId();
    RestoreService rs;
    rs.restore(job, pointId);
    auto restoredDir = src.string() + "_restored_" + pointId;
    REQUIRE(std::filesystem::exists(restoredDir + "/file.txt"));
    std::ifstream in(restoredDir + "/file.txt");
    std::string content;
    in >> content;
    REQUIRE(content == "restore me");
    std::filesystem::remove_all(src);
    std::filesystem::remove_all(dst);
    std::filesystem::remove_all(restoredDir);
}

TEST_CASE("RestoreService throws if point not found", "[RestoreService]") {
    Schedule s; RetentionPolicy r;
    BackupJob job("j", "test", "/a", "/b", s, r);
    RestoreService rs;
    REQUIRE_THROWS_AS(rs.restore(job, "nonexistent"), std::runtime_error);
}

TEST_CASE("RestoreService restores from incremental point", "[RestoreService]") {
    auto src = std::filesystem::temp_directory_path() / "inc_rest_src";
    auto dst = std::filesystem::temp_directory_path() / "inc_rest_dst";
    std::filesystem::create_directories(src);
    std::ofstream(src / "f.txt") << "v1";
    Schedule s; RetentionPolicy r;
    BackupJob job("j2", "test", src, dst, s, r);
    BackupService bs;
    bs.runBackup(job); // full
    std::ofstream(src / "f.txt", std::ios::trunc) << "v2";
    bs.runBackup(job); // inc
    REQUIRE(job.getRestorePoints().size() == 2);
    auto incId = job.getRestorePoints()[1].getId();
    RestoreService rs;
    rs.restore(job, incId);
    auto restoredDir = src.string() + "_restored_" + incId;
    REQUIRE(std::filesystem::exists(restoredDir + "/f.txt"));
    std::ifstream in(restoredDir + "/f.txt");
    std::string content;
    in >> content;
    REQUIRE(content == "v2");
    std::filesystem::remove_all(src);
    std::filesystem::remove_all(dst);
    std::filesystem::remove_all(restoredDir);
}

TEST_CASE("RestoreService restores multiple files", "[RestoreService]") {
    auto src = std::filesystem::temp_directory_path() / "multi_rest_src";
    auto dst = std::filesystem::temp_directory_path() / "multi_rest_dst";
    std::filesystem::create_directories(src);
    std::ofstream(src / "a.txt") << "aaa";
    std::ofstream(src / "b.txt") << "bbb";
    Schedule s; RetentionPolicy r;
    BackupJob job("j3", "test", src, dst, s, r);
    BackupService bs;
    bs.runBackup(job);
    auto pointId = job.getRestorePoints()[0].getId();
    RestoreService rs;
    rs.restore(job, pointId);
    auto restoredDir = src.string() + "_restored_" + pointId;
    REQUIRE(std::filesystem::exists(restoredDir + "/a.txt"));
    REQUIRE(std::filesystem::exists(restoredDir + "/b.txt"));
    std::filesystem::remove_all(src);
    std::filesystem::remove_all(dst);
    std::filesystem::remove_all(restoredDir);
}

TEST_CASE("RestoreService creates restore directory", "[RestoreService]") {
    auto src = std::filesystem::temp_directory_path() / "create_dir_src";
    auto dst = std::filesystem::temp_directory_path() / "create_dir_dst";
    std::filesystem::create_directories(src);
    std::ofstream(src / "f.txt") << "data";
    Schedule s; RetentionPolicy r;
    BackupJob job("j4", "test", src, dst, s, r);
    BackupService bs;
    bs.runBackup(job);
    auto pointId = job.getRestorePoints()[0].getId();
    RestoreService rs;
    rs.restore(job, pointId);
    auto restoredDir = src.string() + "_restored_" + pointId;
    REQUIRE(std::filesystem::is_directory(restoredDir));
    std::filesystem::remove_all(src);
    std::filesystem::remove_all(dst);
    std::filesystem::remove_all(restoredDir);
}