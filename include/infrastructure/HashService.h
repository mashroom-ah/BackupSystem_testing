#pragma once
#include <filesystem>
#include <string>

class HashService {
public:
    std::string calculateHash(const std::filesystem::path& file);
};