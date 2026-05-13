#include "application/RestoreService.h"
#include "infrastructure/CompressionService.h"

#include <filesystem>

void RestoreService::restore(BackupJob& job, const std::string& pointId) {

    CompressionService compressionService;

    for (auto& point : job.getRestorePoints()) {

        if (point.getId() == pointId) {

            std::filesystem::path restoreDir =
                job.getSource().string() + "_restored_" + pointId;

            std::filesystem::create_directories(restoreDir);

            compressionService.decompress(point.getPath(), restoreDir);

            return;
        }
    }

    throw std::runtime_error("Restore point not found");
}
