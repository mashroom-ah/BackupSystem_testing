#include "application/BackupService.h"
#include <filesystem>
#include <iomanip>
#include <sstream>

namespace fs = std::filesystem;

void BackupService::runBackup(BackupJob& job) {

    auto files = fileService.listFiles(job.getSource());

    bool isFirst = job.getRestorePoints().empty();

    std::vector<fs::path> toCopy;

    if (isFirst) {
        toCopy = files; // FULL
        // При первом бэкапе очищаем старые хэши
        job.clearFileHashes();
    } else {
        // передаём хэши из задачи в DiffService
        auto& fileHashes = job.getFileHashes();
        auto changed = diffService.getChangedFiles(files, fileHashes);
        toCopy = changed; // INCREMENTAL
    }

    // Улучшенный нейминг с ID задачи и timestamp
    auto now = std::chrono::system_clock::now();
    auto now_time_t = std::chrono::system_clock::to_time_t(now);

    std::stringstream ss;
    ss << std::put_time(std::localtime(&now_time_t), "%Y%m%d_%H%M%S");
    std::string timestamp_str = ss.str();

    std::string backup_dir_name = job.getId() + "_" + timestamp_str +
                                  (isFirst ? "_FULL" : "_INC");

    fs::path backupDir = job.getTarget() / backup_dir_name;
    fs::create_directories(backupDir);

    for (auto& file : toCopy) {
        fs::path dest = backupDir / fs::relative(file, job.getSource());
        fileService.copyFile(file, dest);
    }

    compressionService.compress(backupDir);

    RestorePoint point(
        backup_dir_name,
        backupDir,
        now,
        isFirst ? BackupType::FULL : BackupType::INCREMENTAL
        );

    job.addRestorePoint(point);

    retentionService.applyRetention(job);

    job.getSchedule().updateLastRun(now);

    // Хэши уже обновлены в DiffService и сохранены в job
    // Они будут сохранены в JSON при следующем вызове repository.save()
}
