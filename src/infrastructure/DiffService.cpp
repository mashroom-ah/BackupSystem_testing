#include "infrastructure/DiffService.h"
#include "infrastructure/HashService.h"

std::vector<std::filesystem::path> DiffService::getChangedFiles(
    const std::vector<std::filesystem::path>& files,
    std::unordered_map<std::string, std::string>& fileHashes) {

    HashService hashService;
    std::vector<std::filesystem::path> changed;

    for (const auto& file : files) {
        auto hash = hashService.calculateHash(file);
        auto pathStr = file.string();

        // Проверяем по переданным хэшам (которые хранятся в BackupJob)
        auto it = fileHashes.find(pathStr);
        if (it == fileHashes.end() || it->second != hash) {
            changed.push_back(file);
            fileHashes[pathStr] = hash;  // Обновляем хэш
        }
    }

    return changed;
}
