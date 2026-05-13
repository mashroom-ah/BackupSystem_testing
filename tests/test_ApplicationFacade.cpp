#include <catch2/catch_test_macros.hpp>
#include "application/ApplicationFacade.h"
#include <filesystem>

TEST_CASE("ApplicationFacade create and get jobs", "[Facade]") {
    auto src = std::filesystem::temp_directory_path() / "facade_src";
    auto dst = std::filesystem::temp_directory_path() / "facade_dst";
    std::filesystem::create_directories(src);
    std::filesystem::create_directories(dst);
    ApplicationFacade facade;
    Schedule s(0, true);
    RetentionPolicy r;
    BackupJob job("f1", "facade_job", src, dst, s, r);
    facade.createJob(job);
    auto& jobs = facade.getJobs();
    REQUIRE(jobs.size() == 1);
    REQUIRE(jobs[0].getId() == "f1");
    std::filesystem::remove_all(src);
    std::filesystem::remove_all(dst);
}

TEST_CASE("ApplicationFacade updateJob", "[Facade]") {
    auto src = std::filesystem::temp_directory_path() / "facade_src2";
    auto dst = std::filesystem::temp_directory_path() / "facade_dst2";
    std::filesystem::create_directories(src);
    std::filesystem::create_directories(dst);
    ApplicationFacade facade;
    Schedule s; RetentionPolicy r;
    BackupJob job("f2", "old", src, dst, s, r);
    facade.createJob(job);
    job.setName("new");
    facade.updateJob(job);
    REQUIRE(facade.getJobs()[0].getName() == "new");
    std::filesystem::remove_all(src);
    std::filesystem::remove_all(dst);
}

TEST_CASE("ApplicationFacade deleteJob", "[Facade]") {
    auto src = std::filesystem::temp_directory_path() / "facade_src3";
    auto dst = std::filesystem::temp_directory_path() / "facade_dst3";
    std::filesystem::create_directories(src);
    std::filesystem::create_directories(dst);
    ApplicationFacade facade;
    Schedule s; RetentionPolicy r;
    facade.createJob(BackupJob("f3", "n", src, dst, s, r));
    facade.deleteJob("f3");
    REQUIRE(facade.getJobs().empty());
    std::filesystem::remove_all(src);
    std::filesystem::remove_all(dst);
}

TEST_CASE("ApplicationFacade runBackup throws on disabled job", "[Facade]") {
    auto src = std::filesystem::temp_directory_path() / "facade_src4";
    auto dst = std::filesystem::temp_directory_path() / "facade_dst4";
    std::filesystem::create_directories(src);
    std::filesystem::create_directories(dst);
    ApplicationFacade facade;
    Schedule s(0, false);
    RetentionPolicy r;
    BackupJob job("f4", "disabled", src, dst, s, r);
    job.disable();
    facade.createJob(job);
    REQUIRE_THROWS_AS(facade.runBackup("f4"), std::runtime_error);
    std::filesystem::remove_all(src);
    std::filesystem::remove_all(dst);
}

TEST_CASE("ApplicationFacade runBackup on non-existent job throws", "[Facade]") {
    ApplicationFacade facade;
    REQUIRE_THROWS_AS(facade.runBackup("no_such_id"), std::runtime_error);
}

TEST_CASE("ApplicationFacade restore throws on invalid point", "[Facade]") {
    auto src = std::filesystem::temp_directory_path() / "facade_src5";
    auto dst = std::filesystem::temp_directory_path() / "facade_dst5";
    std::filesystem::create_directories(src);
    std::filesystem::create_directories(dst);
    ApplicationFacade facade;
    Schedule s; RetentionPolicy r;
    BackupJob job("f5", "test", src, dst, s, r);
    facade.createJob(job);
    REQUIRE_THROWS_AS(facade.restore("f5", "bad_point"), std::runtime_error);
    std::filesystem::remove_all(src);
    std::filesystem::remove_all(dst);
}

TEST_CASE("ApplicationFacade loadJobs loads existing jobs.json", "[Facade]") {
    ApplicationFacade facade;
    REQUIRE_NOTHROW(facade.loadJobs());
}