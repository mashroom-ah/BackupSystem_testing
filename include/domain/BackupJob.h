#pragma once

#include <string>
#include <filesystem>
#include <vector>
#include <unordered_map>

#include "Schedule.h"
#include "RetentionPolicy.h"
#include "RestorePoint.h"

class BackupJob {
private:
    std::string id;
    std::string name;
    std::filesystem::path source;
    std::filesystem::path target;
    Schedule schedule;
    bool enabled;
    RetentionPolicy retention;
    std::vector<RestorePoint> restorePoints;

    // хэши файлов для инкрементального копирования
    std::unordered_map<std::string, std::string> fileHashes;

public:
    BackupJob(const std::string& id,
              const std::string& name,
              const std::filesystem::path& source,
              const std::filesystem::path& target,
              const Schedule& schedule,
              const RetentionPolicy& retention);

    // Геттеры
    std::string getId() const;
    std::string getName() const;

    std::filesystem::path getSource() const;
    std::filesystem::path getTarget() const;

    bool isEnabled() const;

    Schedule& getSchedule();
    const Schedule& getSchedule() const;

    RetentionPolicy& getRetention();
    const RetentionPolicy& getRetention() const;

    std::vector<RestorePoint>& getRestorePoints();
    const std::vector<RestorePoint>& getRestorePoints() const;

    std::unordered_map<std::string, std::string>& getFileHashes();
    const std::unordered_map<std::string, std::string>& getFileHashes() const;
    void setFileHashes(const std::unordered_map<std::string, std::string>& hashes);
    void updateFileHash(const std::string& filePath, const std::string& hash);
    void clearFileHashes();

    // Сеттеры
    void setId(const std::string& newId) { id = newId; }
    void setName(const std::string& newName) { name = newName; }
    void setSource(const std::filesystem::path& newSource) { source = newSource; }
    void setTarget(const std::filesystem::path& newTarget) { target = newTarget; }

    // Действия
    void enable();
    void disable();

    void addRestorePoint(const RestorePoint& point);
};
