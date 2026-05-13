#include <catch2/catch_test_macros.hpp>
#include "infrastructure/JobRepository.h"
#include <filesystem>
#include <fstream>

TEST_CASE("JobRepository save and load empty", "[JobRepository]") {
    auto tmp = std::filesystem::temp_directory_path() / "empty_repo.json";
    JobRepository repo;
    repo.setFilePath(tmp.string());
    std::vector<BackupJob> jobs;
    repo.save(jobs);
    auto loaded = repo.load();
    REQUIRE(loaded.empty());
    std::filesystem::remove(tmp);
}

TEST_CASE("JobRepository save and load one job", "[JobRepository]") {
    auto tmp = std::filesystem::temp_directory_path() / "one_job.json";
    JobRepository repo;
    repo.setFilePath(tmp.string());
    Schedule s(100, true);
    RetentionPolicy r;
    BackupJob job("id1", "test", "/src", "/dst", s, r);
    repo.save({job});
    auto loaded = repo.load();
    REQUIRE(loaded.size() == 1);
    REQUIRE(loaded[0].getId() == "id1");
    REQUIRE(loaded[0].getName() == "test");
    REQUIRE(loaded[0].getSource() == "/src");
    REQUIRE(loaded[0].getTarget() == "/dst");
    REQUIRE(loaded[0].getSchedule().getInterval() == 100);
    REQUIRE(loaded[0].isEnabled() == true);
    std::filesystem::remove(tmp);
}

TEST_CASE("JobRepository persists restore points", "[JobRepository]") {
    auto tmp = std::filesystem::temp_directory_path() / "with_points.json";
    JobRepository repo;
    repo.setFilePath(tmp.string());
    Schedule s; RetentionPolicy r;
    BackupJob job("id", "name", "/src", "/dst", s, r);
    auto now = std::chrono::system_clock::now();
    auto pointPath = std::filesystem::temp_directory_path() / "rp_file";
    std::ofstream(pointPath) << "dummy";
    RestorePoint p("p1", pointPath, now, BackupType::FULL);
    job.addRestorePoint(p);
    repo.save({job});
    auto loaded = repo.load();
    REQUIRE(loaded[0].getRestorePoints().size() == 1);
    REQUIRE(loaded[0].getRestorePoints()[0].getId() == "p1");
    std::filesystem::remove(tmp);
    std::filesystem::remove(pointPath);
}

TEST_CASE("JobRepository persists file hashes", "[JobRepository]") {
    auto tmp = std::filesystem::temp_directory_path() / "hashes.json";
    JobRepository repo;
    repo.setFilePath(tmp.string());
    Schedule s; RetentionPolicy r;
    BackupJob job("id", "name", "/src", "/dst", s, r);
    job.updateFileHash("file.txt", "abc123");
    repo.save({job});
    auto loaded = repo.load();
    REQUIRE(loaded[0].getFileHashes().size() == 1);
    REQUIRE(loaded[0].getFileHashes().at("file.txt") == "abc123");
    std::filesystem::remove(tmp);
}

TEST_CASE("JobRepository loads only existing restore points", "[JobRepository]") {
    auto tmp = std::filesystem::temp_directory_path() / "missing_points.json";
    JobRepository repo;
    repo.setFilePath(tmp.string());
    Schedule s; RetentionPolicy r;
    BackupJob job("id", "name", "/src", "/dst", s, r);
    RestorePoint p("p1", "/non/existent/path", std::chrono::system_clock::now(), BackupType::FULL);
    job.addRestorePoint(p);
    repo.save({job});
    auto loaded = repo.load();
    REQUIRE(loaded[0].getRestorePoints().empty());
    std::filesystem::remove(tmp);
}