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

    int minPoints = std::max(3, policy.getMinRestorePoints());

    // === maxDays ===
    if (policy.getMaxDays() > 0) {
        auto now = std::chrono::system_clock::now();

        points.erase(std::remove_if(points.begin(), points.end(),
                                    [&](const RestorePoint& p) {
                                        auto age = std::chrono::duration_cast<std::chrono::hours>(now - p.getTimestamp()).count() / 24;
                                        return age > policy.getMaxDays();
                                    }), points.end());
    }

    // === maxSize (MB) ===
    if (policy.getMaxStorageSize() > 0) {

        size_t total = 0;
        for (auto& p : points)
            total += fs::file_size(p.getPath());

        while (total > policy.getMaxStorageSize() * 1024 * 1024 && points.size() > minPoints) {

            total -= fs::file_size(points.front().getPath());
            fs::remove_all(points.front().getPath());
            points.erase(points.begin());
        }
    }

    // === гарантируем минимум ===
    while (points.size() > minPoints) {
        fs::remove_all(points.front().getPath());
        points.erase(points.begin());
    }
}
