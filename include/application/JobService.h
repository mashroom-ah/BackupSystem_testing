#pragma once

#include <vector>
#include "domain/BackupJob.h"
#include "infrastructure/JobRepository.h"

class JobService
{
private:
    std::vector<BackupJob> jobs;
    JobRepository repository;

public:
    void createJob(const BackupJob &job);
    void updateJob(const BackupJob &job);
    void deleteJob(const std::string &id);

    std::vector<BackupJob> &getJobs();
    // Для тестов: получить ссылку на репозиторий (чтобы задать временный файл)
    JobRepository &getRepository() { return repository; }

    void load(); // загрузка из json
};
