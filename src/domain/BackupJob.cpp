#include "domain/BackupJob.h"

BackupJob::BackupJob(const std::string& id,
                     const std::string& name,
                     const std::filesystem::path& source,
                     const std::filesystem::path& target,
                     const Schedule& schedule,
                     const RetentionPolicy& retention)
    : id(id), name(name), source(source), target(target),
    schedule(schedule), enabled(schedule.isEnabled()), retention(retention) {}

std::string BackupJob::getId() const { return id; }
std::string BackupJob::getName() const { return name; }

std::filesystem::path BackupJob::getSource() const { return source; }
std::filesystem::path BackupJob::getTarget() const { return target; }

bool BackupJob::isEnabled() const { return enabled; }

Schedule& BackupJob::getSchedule() { return schedule; }
const Schedule& BackupJob::getSchedule() const { return schedule; }

RetentionPolicy& BackupJob::getRetention() { return retention; }
const RetentionPolicy& BackupJob::getRetention() const { return retention; }

std::vector<RestorePoint>& BackupJob::getRestorePoints() { return restorePoints; }
const std::vector<RestorePoint>& BackupJob::getRestorePoints() const { return restorePoints; }

std::unordered_map<std::string, std::string>& BackupJob::getFileHashes() {
    return fileHashes;
}

const std::unordered_map<std::string, std::string>& BackupJob::getFileHashes() const {
    return fileHashes;
}

void BackupJob::setFileHashes(const std::unordered_map<std::string, std::string>& hashes) {
    fileHashes = hashes;
}

void BackupJob::updateFileHash(const std::string& filePath, const std::string& hash) {
    fileHashes[filePath] = hash;
}

void BackupJob::clearFileHashes() {
    fileHashes.clear();
}

void BackupJob::enable() {
    enabled = true;
    schedule.setEnabled(true);
}

void BackupJob::disable() {
    enabled = false;
    schedule.setEnabled(false);
}

void BackupJob::addRestorePoint(const RestorePoint& point) {
    restorePoints.push_back(point);
}
