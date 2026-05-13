#include <catch2/catch_test_macros.hpp>
#include "domain/BackupJob.h"
#include <filesystem>

// ---------- Тесты конструктора и геттеров ----------
TEST_CASE("BackupJob constructor sets all fields correctly", "[BackupJob]") {
    Schedule s(3600, true);
    RetentionPolicy r;
    BackupJob job("id42", "MyImportantJob", "/home/user/data", "/backup/data", s, r);
    REQUIRE(job.getId() == "id42");
    REQUIRE(job.getName() == "MyImportantJob");
    REQUIRE(job.getSource() == "/home/user/data");
    REQUIRE(job.getTarget() == "/backup/data");
    REQUIRE(job.isEnabled() == true);
    REQUIRE(job.getSchedule().getInterval() == 3600);
    REQUIRE(job.getRetention().getMinRestorePoints() == 3);
}

TEST_CASE("BackupJob constructor with disabled schedule", "[BackupJob]") {
    Schedule s(100, false);
    RetentionPolicy r;
    BackupJob job("id", "name", "/s", "/t", s, r);
    REQUIRE_FALSE(job.isEnabled());
    REQUIRE_FALSE(job.getSchedule().isEnabled());
}

TEST_CASE("BackupJob constructor with custom retention", "[BackupJob]") {
    Schedule s(0, true);
    RetentionPolicy r;
    r.changeStorageSize(1024);
    r.changeMaxDays(7);
    r.changeMinRestorePoints(5);
    BackupJob job("id", "name", "/s", "/t", s, r);
    REQUIRE(job.getRetention().getMaxStorageSize() == 1024);
    REQUIRE(job.getRetention().getMaxDays() == 7);
    REQUIRE(job.getRetention().getMinRestorePoints() == 5);
}

TEST_CASE("BackupJob default restore points and hashes are empty", "[BackupJob]") {
    Schedule s; RetentionPolicy r;
    BackupJob job("id", "name", "/s", "/t", s, r);
    REQUIRE(job.getRestorePoints().empty());
    REQUIRE(job.getFileHashes().empty());
}

// ---------- Тесты enable/disable ----------
TEST_CASE("BackupJob enable sets both flags true", "[BackupJob]") {
    Schedule s(0, false);
    RetentionPolicy r;
    BackupJob job("id", "name", "/s", "/t", s, r);
    job.disable();
    REQUIRE_FALSE(job.isEnabled());
    job.enable();
    REQUIRE(job.isEnabled());
    REQUIRE(job.getSchedule().isEnabled());
}

TEST_CASE("BackupJob disable clears both flags", "[BackupJob]") {
    Schedule s(0, true);
    RetentionPolicy r;
    BackupJob job("id", "name", "/s", "/t", s, r);
    job.enable();
    job.disable();
    REQUIRE_FALSE(job.isEnabled());
    REQUIRE_FALSE(job.getSchedule().isEnabled());
}

TEST_CASE("BackupJob multiple enable/disable toggles", "[BackupJob]") {
    Schedule s(0, true);
    RetentionPolicy r;
    BackupJob job("id", "name", "/s", "/t", s, r);
    job.disable();
    job.enable();
    job.disable();
    REQUIRE_FALSE(job.isEnabled());
    job.enable();
    REQUIRE(job.isEnabled());
}

// ---------- Тесты addRestorePoint ----------
TEST_CASE("BackupJob addRestorePoint stores points in order", "[BackupJob]") {
    Schedule s; RetentionPolicy r;
    BackupJob job("id", "name", "/s", "/t", s, r);
    auto t1 = std::chrono::system_clock::now();
    auto t2 = t1 + std::chrono::hours(1);
    RestorePoint p1("p1", "/p1", t1, BackupType::FULL);
    RestorePoint p2("p2", "/p2", t2, BackupType::INCREMENTAL);
    job.addRestorePoint(p1);
    job.addRestorePoint(p2);
    REQUIRE(job.getRestorePoints().size() == 2);
    REQUIRE(job.getRestorePoints()[0].getId() == "p1");
    REQUIRE(job.getRestorePoints()[1].getType() == BackupType::INCREMENTAL);
}

TEST_CASE("BackupJob addRestorePoint can add many", "[BackupJob]") {
    Schedule s; RetentionPolicy r;
    BackupJob job("id", "name", "/s", "/t", s, r);
    for (int i = 0; i < 10; ++i) {
        RestorePoint p("p" + std::to_string(i), "/p", std::chrono::system_clock::now(), BackupType::FULL);
        job.addRestorePoint(p);
    }
    REQUIRE(job.getRestorePoints().size() == 10);
}

// ---------- Тесты file hashes ----------
TEST_CASE("BackupJob updateFileHash adds or updates", "[BackupJob]") {
    Schedule s; RetentionPolicy r;
    BackupJob job("id", "name", "/s", "/t", s, r);
    job.updateFileHash("a.txt", "hash1");
    job.updateFileHash("b.txt", "hash2");
    REQUIRE(job.getFileHashes().size() == 2);
    REQUIRE(job.getFileHashes()["a.txt"] == "hash1");
    job.updateFileHash("a.txt", "newhash");
    REQUIRE(job.getFileHashes().size() == 2);
    REQUIRE(job.getFileHashes()["a.txt"] == "newhash");
}

TEST_CASE("BackupJob clearFileHashes removes all", "[BackupJob]") {
    Schedule s; RetentionPolicy r;
    BackupJob job("id", "name", "/s", "/t", s, r);
    job.updateFileHash("f1", "h1");
    job.updateFileHash("f2", "h2");
    job.clearFileHashes();
    REQUIRE(job.getFileHashes().empty());
}

TEST_CASE("BackupJob setFileHashes replaces existing", "[BackupJob]") {
    Schedule s; RetentionPolicy r;
    BackupJob job("id", "name", "/s", "/t", s, r);
    job.updateFileHash("old", "oldhash");
    std::unordered_map<std::string, std::string> newHashes = {{"new1", "nh1"}, {"new2", "nh2"}};
    job.setFileHashes(newHashes);
    REQUIRE(job.getFileHashes().size() == 2);
    REQUIRE(job.getFileHashes().find("old") == job.getFileHashes().end());
}

// ---------- Тесты сеттеров (id, name, source, target) ----------
TEST_CASE("BackupJob setId changes id", "[BackupJob]") {
    Schedule s; RetentionPolicy r;
    BackupJob job("oldId", "name", "/s", "/t", s, r);
    job.setId("newId");
    REQUIRE(job.getId() == "newId");
}

TEST_CASE("BackupJob setName changes name", "[BackupJob]") {
    Schedule s; RetentionPolicy r;
    BackupJob job("id", "oldName", "/s", "/t", s, r);
    job.setName("newName");
    REQUIRE(job.getName() == "newName");
}

TEST_CASE("BackupJob setSource changes source", "[BackupJob]") {
    Schedule s; RetentionPolicy r;
    BackupJob job("id", "name", "/old/src", "/t", s, r);
    job.setSource("/new/src");
    REQUIRE(job.getSource() == "/new/src");
}

TEST_CASE("BackupJob setTarget changes target", "[BackupJob]") {
    Schedule s; RetentionPolicy r;
    BackupJob job("id", "name", "/s", "/old/dst", s, r);
    job.setTarget("/new/dst");
    REQUIRE(job.getTarget() == "/new/dst");
}