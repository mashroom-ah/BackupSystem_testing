#include "infrastructure/RetentionService.h"
#include <algorithm>
#include <filesystem>

namespace fs = std::filesystem;

static size_t getDirectorySize(const fs::path& dir) {
    size_t size = 0;
    if (!fs::exists(dir)) return 0;
    if (fs::is_regular_file(dir)) return fs::file_size(dir);
    for (const auto& entry : fs::recursive_directory_iterator(dir)) {
        if (fs::is_regular_file(entry.path()))
            size += fs::file_size(entry.path());
    }
    return size;
}

void RetentionService::applyRetention(BackupJob& job) {
    auto& points = job.getRestorePoints();
    auto& policy = job.getRetention();

    std::sort(points.begin(), points.end(),
              [](const RestorePoint& a, const RestorePoint& b) {
                  return a.getTimestamp() < b.getTimestamp();
              });

    int minPoints = std::max(1, policy.getMinRestorePoints());

    if (policy.getMaxDays() > 0) {
        auto now = std::chrono::system_clock::now();
        auto it = points.begin();
        while (it != points.end()) {
            auto ageHours = std::chrono::duration_cast<std::chrono::hours>(now - it->getTimestamp()).count();
            if (ageHours / 24 >= policy.getMaxDays()) {
                if (fs::exists(it->getPath()))
                    fs::remove_all(it->getPath());
                it = points.erase(it);
            } else {
                ++it;
            }
        }
    }

    if (policy.getMaxStorageSize() > 0) {
        size_t total = 0;
        for (auto& p : points) {
            total += getDirectorySize(p.getPath());
        }
        while (total > static_cast<size_t>(policy.getMaxStorageSize()) * 1024 * 1024 && points.size() > static_cast<size_t>(minPoints)) {
            if (fs::exists(points.front().getPath())) {
                total -= getDirectorySize(points.front().getPath());
                fs::remove_all(points.front().getPath());
            }
            points.erase(points.begin());
        }
    }

    while (points.size() > static_cast<size_t>(minPoints)) {
        if (fs::exists(points.front().getPath()))
            fs::remove_all(points.front().getPath());
        points.erase(points.begin());
    }
}