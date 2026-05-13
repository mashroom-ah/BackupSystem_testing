#include <catch2/catch_test_macros.hpp>
#include "application/JobService.h"
#include <filesystem>

TEST_CASE("JobService create and get jobs", "[JobService]") {
    JobService service;
    auto tempFile = std::filesystem::temp_directory_path() / "jobservice_test.json";
    service.getRepository().setFilePath(tempFile.string());
    Schedule s(60, true);
    RetentionPolicy r;
    BackupJob job("id1", "job1", "/src", "/dst", s, r);
    service.createJob(job);
    REQUIRE(service.getJobs().size() == 1);
    REQUIRE(service.getJobs()[0].getId() == "id1");
    std::filesystem::remove(tempFile);
}

TEST_CASE("JobService updateJob", "[JobService]") {
    JobService service;
    auto tempFile = std::filesystem::temp_directory_path() / "update_test.json";
    service.getRepository().setFilePath(tempFile.string());
    Schedule s; RetentionPolicy r;
    BackupJob job("id2", "name", "/s", "/d", s, r);
    service.createJob(job);
    job.disable();
    service.updateJob(job);
    REQUIRE_FALSE(service.getJobs()[0].isEnabled());
    std::filesystem::remove(tempFile);
}

TEST_CASE("JobService deleteJob", "[JobService]") {
    JobService service;
    auto tempFile = std::filesystem::temp_directory_path() / "delete_test.json";
    service.getRepository().setFilePath(tempFile.string());
    Schedule s; RetentionPolicy r;
    service.createJob(BackupJob("id3", "n", "/s", "/d", s, r));
    REQUIRE(service.getJobs().size() == 1);
    service.deleteJob("id3");
    REQUIRE(service.getJobs().empty());
    std::filesystem::remove(tempFile);
}

TEST_CASE("JobService load from file", "[JobService]") {
    JobService service;
    auto tempFile = std::filesystem::temp_directory_path() / "load_test.json";
    service.getRepository().setFilePath(tempFile.string());
    Schedule s; RetentionPolicy r;
    BackupJob job("id4", "job4", "/src4", "/dst4", s, r);
    service.createJob(job);
    JobService service2;
    service2.getRepository().setFilePath(tempFile.string());
    service2.load();
    REQUIRE(service2.getJobs().size() == 1);
    REQUIRE(service2.getJobs()[0].getName() == "job4");
    std::filesystem::remove(tempFile);
}

TEST_CASE("JobService create multiple jobs", "[JobService]") {
    JobService service;
    auto tempFile = std::filesystem::temp_directory_path() / "multi.json";
    service.getRepository().setFilePath(tempFile.string());
    Schedule s; RetentionPolicy r;
    for (int i = 0; i < 5; ++i) {
        BackupJob job("id" + std::to_string(i), "job" + std::to_string(i), "/s", "/d", s, r);
        service.createJob(job);
    }
    REQUIRE(service.getJobs().size() == 5);
    std::filesystem::remove(tempFile);
}

TEST_CASE("JobService update non-existent job does nothing", "[JobService]") {
    JobService service;
    auto tempFile = std::filesystem::temp_directory_path() / "nonexistent_update.json";
    service.getRepository().setFilePath(tempFile.string());
    Schedule s; RetentionPolicy r;
    BackupJob job("idX", "name", "/s", "/d", s, r);
    service.updateJob(job);
    REQUIRE(service.getJobs().empty());
    std::filesystem::remove(tempFile);
}