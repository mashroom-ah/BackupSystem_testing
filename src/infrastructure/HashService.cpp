#include "infrastructure/HashService.h"
#include <fstream>
#include <zlib.h>

std::string HashService::calculateHash(const std::filesystem::path& file) {

    std::ifstream ifs(file, std::ios::binary);
    if (!ifs) return "";

    uLong crc = crc32(0L, Z_NULL, 0);

    char buffer[4096];
    while (ifs.read(buffer, sizeof(buffer)) || ifs.gcount()) {
        crc = crc32(crc,
                    reinterpret_cast<const Bytef*>(buffer),
                    ifs.gcount());
    }

    return std::to_string(crc);
}
