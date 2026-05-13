#include "domain/RestorePoint.h"

RestorePoint::RestorePoint(const std::string& id,
                           const std::filesystem::path& path,
                           std::chrono::system_clock::time_point timestamp,
                           BackupType type)
    : id(id),
    path(path),
    timestamp(timestamp),
    type(type) {}

// Геттеры
std::string RestorePoint::getId() const {
    return id;
}

std::filesystem::path RestorePoint::getPath() const {
    return path;
}

std::chrono::system_clock::time_point RestorePoint::getTimestamp() const {
    return timestamp;
}

BackupType RestorePoint::getType() const {
    return type;
}
