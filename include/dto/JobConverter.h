#pragma once

#include "dto/BackupJobDTO.h"
#include "domain/BackupJob.h"

class JobConverter {
public:
    static BackupJob fromDTO(const BackupJobDTO& dto);
    static BackupJobDTO toDTO(const BackupJob& job);
};
