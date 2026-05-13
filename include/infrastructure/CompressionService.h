#pragma once

#include <filesystem>

class CompressionService {
public:
    void compress(const std::filesystem::path& source);

    void decompress(const std::filesystem::path& source,
                    const std::filesystem::path& destination);
};
