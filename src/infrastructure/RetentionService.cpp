#include "infrastructure/RetentionService.h"
#include <algorithm>
#include <filesystem>

namespace fs = std::filesystem;

void RetentionService::applyRetention(BackupJob& job) {
    auto& points = job.getRestorePoints();
    auto& policy = job.getRetention();

    std::sort(points.begin(), points.end(),
              [](const RestorePoint& a, const RestorePoint& b) {
                  return a.getTimestamp() < b.getTimestamp();
              });

    int minPoints = std::max(1, policy.getMinRestorePoints());

    // Удаление по максимальному возрасту (maxDays)
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

    // Удаление по максимальному размеру хранилища (maxStorageSize)
    if (policy.getMaxStorageSize() > 0) {
        size_t total = 0;
        for (auto& p : points) {
            if (fs::exists(p.getPath()))
                total += fs::file_size(p.getPath());
        }
        while (total > static_cast<size_t>(policy.getMaxStorageSize()) * 1024 * 1024 && points.size() > static_cast<size_t>(minPoints)) {
            if (fs::exists(points.front().getPath())) {
                total -= fs::file_size(points.front().getPath());
                fs::remove_all(points.front().getPath());
            }
            points.erase(points.begin());
        }
    }

    // Гарантия минимального количества точек
    while (points.size() > static_cast<size_t>(minPoints)) {
        if (fs::exists(points.front().getPath()))
            fs::remove_all(points.front().getPath());
        points.erase(points.begin());
    }
}