#pragma once

#include <vector>
#include "domain/RestorePoint.h"

class StorageService {
public:
    void compress(const std::filesystem::path& path);
};
