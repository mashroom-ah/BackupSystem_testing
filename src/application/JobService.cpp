#include "application/JobService.h"
#include <algorithm>

void JobService::createJob(const BackupJob& job) {
    jobs.push_back(job);
    repository.save(jobs);
}

void JobService::updateJob(const BackupJob& job) {
    for (auto& j : jobs) {
        if (j.getId() == job.getId()) {
            // Сохраняем точки восстановления из старой задачи
            // Они уже скопированы в новую в getJob()
            j = job;
            break;
        }
    }
    repository.save(jobs);
}

void JobService::deleteJob(const std::string& id) {
    jobs.erase(std::remove_if(jobs.begin(), jobs.end(),
                              [&](const BackupJob& j) {
                                  return j.getId() == id;
                              }), jobs.end());

    repository.save(jobs);
}

std::vector<BackupJob>& JobService::getJobs() {
    return jobs;
}

void JobService::load() {
    jobs = repository.load();
}
