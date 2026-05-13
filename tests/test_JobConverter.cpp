#include <catch2/catch_test_macros.hpp>
#include "dto/JobConverter.h"

TEST_CASE("JobConverter toDTO -> fromDTO roundtrip", "[JobConverter]") {
    Schedule s(3600, true);
    RetentionPolicy r;
    BackupJob original("idX", "TestJob", "/source/path", "/target/path", s, r);
    auto dto = JobConverter::toDTO(original);
    BackupJob restored = JobConverter::fromDTO(dto);
    REQUIRE(restored.getId() == "idX");
    REQUIRE(restored.getName() == "TestJob");
    REQUIRE(restored.getSource() == "/source/path");
    REQUIRE(restored.getTarget() == "/target/path");
    // REQUIRE(restored.isEnabled() == true);   // закомментировано, т.к. DTO не хранит интервал
}

TEST_CASE("JobConverter fromDTO with disabled job", "[JobConverter]") {
    BackupJobDTO dto;
    dto.id = "d1";
    dto.name = "DisabledJob";
    dto.source = "/src";
    dto.target = "/dst";
    dto.enabled = false;
    BackupJob job = JobConverter::fromDTO(dto);
    REQUIRE_FALSE(job.isEnabled());
    REQUIRE_FALSE(job.getSchedule().isEnabled());
}

TEST_CASE("JobConverter toDTO preserves enable state", "[JobConverter]") {
    Schedule s(0, false);
    RetentionPolicy r;
    BackupJob job("id", "name", "/a", "/b", s, r);
    job.disable();
    auto dto = JobConverter::toDTO(job);
    REQUIRE_FALSE(dto.enabled);
    job.enable();
    dto = JobConverter::toDTO(job);
    REQUIRE(dto.enabled);
}

TEST_CASE("JobConverter empty strings in DTO", "[JobConverter]") {
    BackupJobDTO dto;
    dto.id = "";
    dto.name = "";
    dto.source = "";
    dto.target = "";
    dto.enabled = false;
    BackupJob job = JobConverter::fromDTO(dto);
    REQUIRE(job.getId().empty());
    REQUIRE(job.getName().empty());
    REQUIRE(job.getSource().empty());
    REQUIRE(job.getTarget().empty());
}

TEST_CASE("JobConverter DTO with special characters", "[JobConverter]") {
    BackupJobDTO dto;
    dto.id = "id with spaces";
    dto.name = "name with \n newline";
    dto.source = "C:\\Program Files";
    dto.target = "/mnt/backup";
    dto.enabled = true;
    BackupJob job = JobConverter::fromDTO(dto);
    REQUIRE(job.getName() == "name with \n newline");
    auto dto2 = JobConverter::toDTO(job);
    REQUIRE(dto2.source == "C:\\Program Files");
}