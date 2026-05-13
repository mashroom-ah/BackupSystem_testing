#include "infrastructure/CompressionService.h"
#include <filesystem>
#include <fstream>
#include <zlib.h>

void CompressionService::compress(const std::filesystem::path& source) {
    for (auto& entry : std::filesystem::recursive_directory_iterator(source)) {
        if (!entry.is_regular_file())
            continue;
        auto filePath = entry.path();
        auto gzPath = filePath.string() + ".gz";
        std::ifstream in(filePath, std::ios::binary);
        if (!in)
            continue;
        gzFile out = gzopen(gzPath.c_str(), "wb");
        if (!out)
            continue;
        char buffer[4096];
        while (in.read(buffer, sizeof(buffer)) || in.gcount()) {
            gzwrite(out, buffer, static_cast<unsigned int>(in.gcount()));
        }
        gzclose(out);
        in.close();
        std::filesystem::remove(filePath);
    }
}

void CompressionService::decompress(const std::filesystem::path& source,
                                    const std::filesystem::path& destination) {
    // Создаём целевую директорию, если её нет
    std::filesystem::create_directories(destination);
    for (auto& entry : std::filesystem::recursive_directory_iterator(source)) {
        if (!entry.is_regular_file())
            continue;
        auto filePath = entry.path();
        if (filePath.extension() != ".gz")
            continue;
        std::string outPath = (destination / filePath.stem()).string();
        // Убедимся, что родительская директория для outPath существует
        std::filesystem::create_directories(std::filesystem::path(outPath).parent_path());
        gzFile in = gzopen(filePath.string().c_str(), "rb");
        if (!in)
            continue;
        std::ofstream out(outPath, std::ios::binary);
        if (!out) {
            gzclose(in);
            continue;
        }
        char buffer[4096];
        int bytes = 0;
        while ((bytes = gzread(in, buffer, sizeof(buffer))) > 0) {
            out.write(buffer, bytes);
        }
        out.close();
        gzclose(in);
    }
}