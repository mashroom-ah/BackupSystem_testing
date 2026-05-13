#include "infrastructure/FileService.h"
#include <filesystem>
#include <stdexcept>

namespace fs = std::filesystem;

std::vector<fs::path> FileService::listFiles(const fs::path& dir) {
    std::vector<fs::path> files;

    for (auto& p : fs::recursive_directory_iterator(dir)) {
        if (fs::is_regular_file(p))
            files.push_back(p.path());
    }

    return files;
}

void FileService::copyFile(const fs::path& src, const fs::path& dest) {
    try {
        fs::create_directories(dest.parent_path());
        fs::copy_file(src, dest, fs::copy_options::overwrite_existing);
    }
    catch (...) {
        throw std::runtime_error("Закройте файл перед резервным копированием");
    }
}
