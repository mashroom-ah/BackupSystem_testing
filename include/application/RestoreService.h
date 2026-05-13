#pragma once

#include "domain/BackupJob.h"

class RestoreService {
public:
    void restore(BackupJob& job, const std::string& pointId);
};
