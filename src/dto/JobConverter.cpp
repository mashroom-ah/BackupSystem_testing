#include "dto/JobConverter.h"

BackupJob JobConverter::fromDTO(const BackupJobDTO& dto) {
    return BackupJob(
        dto.id,
        dto.name,
        dto.source,
        dto.target,
        Schedule(),
        RetentionPolicy()
        );
}

BackupJobDTO JobConverter::toDTO(const BackupJob& job) {
    BackupJobDTO dto;
    dto.id = job.getId();
    dto.name = job.getName();
    dto.source = job.getSource().string();
    dto.target = job.getTarget().string();
    dto.enabled = job.isEnabled();
    return dto;
}
