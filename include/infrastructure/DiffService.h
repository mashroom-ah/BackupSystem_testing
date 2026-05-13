#pragma once
#include <vector>
#include <filesystem>
#include <unordered_map>
#include <string>

class DiffService {
public:
    std::vector<std::filesystem::path> getChangedFiles(
        const std::vector<std::filesystem::path>& files,
        std::unordered_map<std::string, std::string>& fileHashes);
};