#include <catch2/catch_test_macros.hpp>
#include "application/ApplicationFacade.h"
#include <filesystem>
#include <fstream>
#include <thread>

SCENARIO("Complete lifecycle: create, edit, delete a job", "[scenario]") {
    GIVEN("a new backup job") {
        ApplicationFacade facade;
        std::string jobId = "lifecycle_job";
        Schedule s(3600, true);
        RetentionPolicy r;
        BackupJob job(jobId, "InitialName", "/source", "/target", s, r);
        WHEN("job is created") {
            facade.createJob(job);
            THEN("it appears in the list") {
                auto& jobs = facade.getJobs();
                REQUIRE(jobs.size() == 1);
                REQUIRE(jobs[0].getName() == "InitialName");
            }
            AND_WHEN("job is edited (name changed)") {
                job.setName("NewName");
                facade.updateJob(job);
                THEN("name updates") {
                    REQUIRE(facade.getJobs()[0].getName() == "NewName");
                }
                AND_WHEN("job is deleted") {
                    facade.deleteJob(jobId);
                    THEN("list becomes empty") {
                        REQUIRE(facade.getJobs().empty());
                    }
                }
            }
        }
    }
}

SCENARIO("Scheduled backup respects interval", "[scenario]") {
    GIVEN("a job with interval 2 seconds") {
        auto src = std::filesystem::temp_directory_path() / "schedule_src";
        auto dst = std::filesystem::temp_directory_path() / "schedule_dst";
        std::filesystem::create_directories(src);
        std::ofstream(src / "f.txt") << "data";
        ApplicationFacade facade;
        Schedule s(2, true);
        RetentionPolicy r;
        BackupJob job("sched1", "scheduled", src, dst, s, r);
        facade.createJob(job);
        WHEN("time passes less than interval") {
            auto& j = facade.getJobs()[0];
            auto lastRun = j.getSchedule().getLastRun();
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            REQUIRE_FALSE(j.getSchedule().shouldRun(std::chrono::system_clock::now()));
            AND_WHEN("time passes more than interval") {
                std::this_thread::sleep_for(std::chrono::seconds(2));
                REQUIRE(j.getSchedule().shouldRun(std::chrono::system_clock::now()));
                facade.runBackup("sched1");
                THEN("a restore point is created") {
                    REQUIRE(facade.getJobs()[0].getRestorePoints().size() == 1);
                }
            }
        }
        std::filesystem::remove_all(src);
        std::filesystem::remove_all(dst);
    }
}

SCENARIO("Full and incremental backup with restore from latest", "[scenario]") {
    GIVEN("a source with one file") {
        auto src = std::filesystem::temp_directory_path() / "inc_src";
        auto dst = std::filesystem::temp_directory_path() / "inc_dst";
        std::filesystem::create_directories(src);
        std::ofstream(src / "data.txt") << "version1";
        ApplicationFacade facade;
        Schedule s; RetentionPolicy r;
        BackupJob job("inc1", "inc_test", src, dst, s, r);
        facade.createJob(job);
        WHEN("first backup runs") {
            facade.runBackup("inc1");
            auto& points = facade.getJobs()[0].getRestorePoints();
            REQUIRE(points.size() == 1);
            REQUIRE(points[0].getType() == BackupType::FULL);
            AND_WHEN("file changes and second backup runs") {
                std::ofstream(src / "data.txt", std::ios::trunc) << "version2";
                facade.runBackup("inc1");
                REQUIRE(points.size() == 2);
                REQUIRE(points[1].getType() == BackupType::INCREMENTAL);
                AND_WHEN("restoring from the latest point") {
                    facade.restore("inc1", points[1].getId());
                    auto restoredDir = src.string() + "_restored_" + points[1].getId();
                    THEN("restored file has version2") {
                        std::ifstream in(restoredDir + "/data.txt");
                        std::string content;
                        in >> content;
                        REQUIRE(content == "version2");
                        std::filesystem::remove_all(restoredDir);
                    }
                }
            }
        }
        std::filesystem::remove_all(src);
        std::filesystem::remove_all(dst);
    }
}

SCENARIO("Retention policy by max size", "[scenario]") {
    GIVEN("a job with max storage size 1 MB and min points 1") {
        auto src = std::filesystem::temp_directory_path() / "size_src";
        auto dst = std::filesystem::temp_directory_path() / "size_dst";
        std::filesystem::create_directories(src);
        std::ofstream(src / "f.txt") << "x";
        ApplicationFacade facade;
        Schedule s; RetentionPolicy r;
        r.changeStorageSize(1); // 1 MB
        r.changeMinRestorePoints(1);
        BackupJob job("size1", "size_test", src, dst, s, r);
        facade.createJob(job);
        WHEN("we create multiple backup points that exceed size") {
            facade.runBackup("size1");
            std::ofstream(src / "f.txt", std::ios::trunc) << std::string(800 * 1024, 'A');
            facade.runBackup("size1");
            // Now total ~1.6 MB, should delete oldest
            auto& points = facade.getJobs()[0].getRestorePoints();
            THEN("only one point remains (the newest)") {
                REQUIRE(points.size() == 1);
            }
        }
        std::filesystem::remove_all(src);
        std::filesystem::remove_all(dst);
    }
}

SCENARIO("Retention policy by max days", "[scenario]") {
    GIVEN("a job with max days = 1") {
        auto src = std::filesystem::temp_directory_path() / "days_src";
        auto dst = std::filesystem::temp_directory_path() / "days_dst";
        std::filesystem::create_directories(src);
        std::ofstream(src / "f.txt") << "data";
        ApplicationFacade facade;
        Schedule s; RetentionPolicy r;
        r.changeMaxDays(1);
        BackupJob job("days1", "days_test", src, dst, s, r);
        facade.createJob(job);
        WHEN("a backup point older than 1 day exists") {
            auto& j = facade.getJobs()[0];
            auto oldTime = std::chrono::system_clock::now() - std::chrono::hours(25);
            RestorePoint oldPoint("old", dst / "old", oldTime, BackupType::FULL);
            j.addRestorePoint(oldPoint);
            facade.runBackup("days1");
            auto& points = j.getRestorePoints();
            THEN("old point is removed") {
                bool oldExists = false;
                for (auto& p : points) {
                    if (p.getId() == "old") oldExists = true;
                }
                REQUIRE_FALSE(oldExists);
                REQUIRE(points.size() == 1);
            }
        }
        std::filesystem::remove_all(src);
        std::filesystem::remove_all(dst);
    }
}

SCENARIO("Error handling: backup when source file is locked", "[scenario]") {
    GIVEN("a source file that is open exclusively") {
        auto src = std::filesystem::temp_directory_path() / "locked_src";
        auto dst = std::filesystem::temp_directory_path() / "locked_dst";
        std::filesystem::create_directories(src);
        std::ofstream locked(src / "locked.txt");
        locked << "data";
        ApplicationFacade facade;
        Schedule s; RetentionPolicy r;
        BackupJob job("lock1", "lock_test", src, dst, s, r);
        facade.createJob(job);
        WHEN("backup is attempted") {
            try {
                facade.runBackup("lock1");
            } catch (const std::runtime_error& e) {
                THEN("exception is caught") {
                    REQUIRE(std::string(e.what()).find("Закройте файл") != std::string::npos);
                }
            }
        }
        locked.close();
        std::filesystem::remove_all(src);
        std::filesystem::remove_all(dst);
    }
}