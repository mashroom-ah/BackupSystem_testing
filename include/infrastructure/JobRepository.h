#pragma once

#include <vector>
#include "domain/BackupJob.h"

class JobRepository {
public:
    void save(const std::vector<BackupJob>& jobs);
    std::vector<BackupJob> load();

    // Для тестов: задать путь к JSON-файлу
    void setFilePath(const std::string& path) { filePath = path; }

private:
    std::string filePath = "jobs.json";   // по умолчанию
};