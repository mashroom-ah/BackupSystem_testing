#include "application/ApplicationFacade.h"

ApplicationFacade::ApplicationFacade() {}

void ApplicationFacade::createJob(const BackupJob& job) {
    jobService.createJob(job);

    // СРАЗУ ДЕЛАЕМ ПЕРВЫЙ FULL BACKUP (FR-07)
    auto& jobs = jobService.getJobs();

    for (auto& j : jobs) {
        if (j.getId() == job.getId()) {
            if (j.isEnabled()) {
                backupService.runBackup(j);
                jobService.updateJob(j);  // Сохраняем после бэкапа
            }
            break;
        }
    }
}

void ApplicationFacade::updateJob(const BackupJob& job) {
    jobService.updateJob(job);
}

void ApplicationFacade::deleteJob(const std::string& id) {
    jobService.deleteJob(id);
}

void ApplicationFacade::runBackup(const std::string& id) {
    auto& jobs = jobService.getJobs();

    for (auto& job : jobs) {
        if (job.getId() == id && job.isEnabled()) {
            backupService.runBackup(job);
            jobService.updateJob(job);  // СОХРАНЯЕМ ИЗМЕНЕНИЯ
            return;
        }
    }

    throw std::runtime_error("Задача не найдена или отключена");
}

void ApplicationFacade::restore(const std::string& jobId, const std::string& pointId) {
    auto& jobs = jobService.getJobs();

    for (auto& job : jobs) {
        if (job.getId() == jobId) {
            for (auto& point : job.getRestorePoints()) {
                if (point.getId() == pointId) {
                    restoreService.restore(job, point.getId());
                    return;
                }
            }
        }
    }

    throw std::runtime_error("Точка восстановления не найдена");
}

std::vector<BackupJob>& ApplicationFacade::getJobs() {
    return jobService.getJobs();
}

void ApplicationFacade::loadJobs() {
    jobService.load();
}
