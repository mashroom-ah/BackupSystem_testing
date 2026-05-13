#pragma once
#include <string>

class BackupJobDTO {
public:
    std::string id;
    std::string name;
    std::string source;
    std::string target;
    bool enabled;
};
