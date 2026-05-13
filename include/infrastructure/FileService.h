#pragma once

#include <filesystem>
#include <vector>

class FileService {
public:
    std::vector<std::filesystem::path> listFiles(const std::filesystem::path& dir);
    void copyFile(const std::filesystem::path& src, const std::filesystem::path& dest);
};
