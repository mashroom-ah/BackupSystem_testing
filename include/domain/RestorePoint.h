#pragma once

#include <string>
#include <filesystem>
#include <chrono>
#include "BackupType.h"

class RestorePoint {
private:
    std::string id;
    std::filesystem::path path;
    std::chrono::system_clock::time_point timestamp;
    BackupType type;

public:
    // Конструктор по умолчанию
    RestorePoint() = default;

    RestorePoint(const std::string& id,
                 const std::filesystem::path& path,
                 std::chrono::system_clock::time_point timestamp,
                 BackupType type);

    // Геттеры
    std::string getId() const;
    std::filesystem::path getPath() const;
    std::chrono::system_clock::time_point getTimestamp() const;
    BackupType getType() const;
};
