#pragma once

#include "domain/BackupJob.h"

class RetentionService {
public:
    void applyRetention(BackupJob& job);
};
