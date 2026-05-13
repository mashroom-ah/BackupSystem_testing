#include <catch2/catch_test_macros.hpp>
#include "application/StorageService.h"
#include <fstream>
#include <filesystem>

TEST_CASE("StorageService compress does not throw on non-existent path", "[StorageService]") {
    auto tmp = std::filesystem::temp_directory_path() / "empty_storage";
    std::filesystem::create_directories(tmp);
    StorageService ss;
    REQUIRE_NOTHROW(ss.compress(tmp));
    std::filesystem::remove_all(tmp);
}

TEST_CASE("StorageService compress on empty directory", "[StorageService]") {
    auto tmp = std::filesystem::temp_directory_path() / "empty_storage";
    std::filesystem::create_directories(tmp);
    StorageService ss;
    REQUIRE_NOTHROW(ss.compress(tmp));
    std::filesystem::remove_all(tmp);
}

TEST_CASE("StorageService compress on directory with files", "[StorageService]") {
    auto tmp = std::filesystem::temp_directory_path() / "storage_files";
    std::filesystem::create_directories(tmp);
    std::ofstream(tmp / "a.txt") << "hello";
    StorageService ss;
    REQUIRE_NOTHROW(ss.compress(tmp));
    REQUIRE(std::filesystem::exists(tmp / "a.txt.gz"));
    std::filesystem::remove_all(tmp);
}

TEST_CASE("StorageService compress on subdirectory", "[StorageService]") {
    auto tmp = std::filesystem::temp_directory_path() / "storage_sub";
    std::filesystem::create_directories(tmp / "sub");
    std::ofstream(tmp / "sub/data.bin") << "binary";
    StorageService ss;
    REQUIRE_NOTHROW(ss.compress(tmp));
    REQUIRE(std::filesystem::exists(tmp / "sub/data.bin.gz"));
    std::filesystem::remove_all(tmp);
}

TEST_CASE("StorageService compress on empty file", "[StorageService]") {
    auto tmp = std::filesystem::temp_directory_path() / "storage_empty";
    std::filesystem::create_directories(tmp);
    std::ofstream(tmp / "empty.txt") << "";
    StorageService ss;
    REQUIRE_NOTHROW(ss.compress(tmp));
    REQUIRE(std::filesystem::exists(tmp / "empty.txt.gz"));
    std::filesystem::remove_all(tmp);
}

TEST_CASE("StorageService compress on large file (no crash)", "[StorageService]") {
    auto tmp = std::filesystem::temp_directory_path() / "storage_large";
    std::filesystem::create_directories(tmp);
    std::ofstream ofs(tmp / "large.bin", std::ios::binary);
    for (int i = 0; i < 1024 * 1024; ++i) ofs << 'X';
    ofs.close();
    StorageService ss;
    REQUIRE_NOTHROW(ss.compress(tmp));
    std::filesystem::remove_all(tmp);
}