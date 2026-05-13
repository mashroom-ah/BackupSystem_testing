#pragma once

#include "JobService.h"
#include "BackupService.h"
#include "RestoreService.h"

class ApplicationFacade {
private:
    JobService jobService;
    BackupService backupService;
    RestoreService restoreService;

public:
    ApplicationFacade();

    void createJob(const BackupJob& job);
    void deleteJob(const std::string& id);
    void updateJob(const BackupJob& job);

    void runBackup(const std::string& id);
    void restore(const std::string& jobId, const std::string& pointId);

    void loadJobs();

    std::vector<BackupJob>& getJobs();
};
