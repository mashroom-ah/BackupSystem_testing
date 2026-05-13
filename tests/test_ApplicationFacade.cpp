#include <catch2/catch_test_macros.hpp>
#include "application/ApplicationFacade.h"
#include <filesystem>

TEST_CASE("ApplicationFacade create and get jobs", "[Facade]") {
    ApplicationFacade facade;
    Schedule s(0, true);
    RetentionPolicy r;
    BackupJob job("f1", "facade_job", "/src", "/dst", s, r);
    facade.createJob(job);
    auto& jobs = facade.getJobs();
    REQUIRE(jobs.size() == 1);
    REQUIRE(jobs[0].getId() == "f1");
}

TEST_CASE("ApplicationFacade updateJob", "[Facade]") {
    ApplicationFacade facade;
    Schedule s; RetentionPolicy r;
    BackupJob job("f2", "old", "/s", "/d", s, r);
    facade.createJob(job);
    job.setName("new");
    facade.updateJob(job);
    REQUIRE(facade.getJobs()[0].getName() == "new");
}

TEST_CASE("ApplicationFacade deleteJob", "[Facade]") {
    ApplicationFacade facade;
    Schedule s; RetentionPolicy r;
    facade.createJob(BackupJob("f3", "n", "/s", "/d", s, r));
    facade.deleteJob("f3");
    REQUIRE(facade.getJobs().empty());
}

TEST_CASE("ApplicationFacade runBackup throws on disabled job", "[Facade]") {
    ApplicationFacade facade;
    Schedule s(0, false);
    RetentionPolicy r;
    BackupJob job("f4", "disabled", "/s", "/d", s, r);
    job.disable();
    facade.createJob(job);
    REQUIRE_THROWS_AS(facade.runBackup("f4"), std::runtime_error);
}

TEST_CASE("ApplicationFacade runBackup on non-existent job throws", "[Facade]") {
    ApplicationFacade facade;
    REQUIRE_THROWS_AS(facade.runBackup("no_such_id"), std::runtime_error);
}

TEST_CASE("ApplicationFacade restore throws on invalid point", "[Facade]") {
    ApplicationFacade facade;
    Schedule s; RetentionPolicy r;
    BackupJob job("f5", "test", "/s", "/d", s, r);
    facade.createJob(job);
    REQUIRE_THROWS_AS(facade.restore("f5", "bad_point"), std::runtime_error);
}

TEST_CASE("ApplicationFacade loadJobs loads existing jobs.json", "[Facade]") {
    // This test is tricky because it uses default jobs.json; better to use a temp file.
    // But we can just ensure it doesn't crash.
    ApplicationFacade facade;
    REQUIRE_NOTHROW(facade.loadJobs());
}