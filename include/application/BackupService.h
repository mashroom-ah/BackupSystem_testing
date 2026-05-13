#pragma once

#include "domain/BackupJob.h"
#include "infrastructure/FileService.h"
#include "infrastructure/DiffService.h"
#include "infrastructure/CompressionService.h"
#include "infrastructure/RetentionService.h"

class BackupService {
private:
    FileService fileService;
    DiffService diffService;
    CompressionService compressionService;
    RetentionService retentionService;

public:
    void runBackup(BackupJob& job);
};
