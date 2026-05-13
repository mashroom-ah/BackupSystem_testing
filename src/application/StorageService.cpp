#include "application/StorageService.h"
#include "infrastructure/CompressionService.h"

void StorageService::compress(const std::filesystem::path& path) {
    CompressionService().compress(path);
}
