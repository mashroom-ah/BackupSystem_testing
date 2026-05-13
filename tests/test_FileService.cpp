#include <catch2/catch_test_macros.hpp>
#include "infrastructure/FileService.h"
#include <filesystem>
#include <fstream>
#include <thread>

namespace fs = std::filesystem;

TEST_CASE("FileService listFiles on empty directory", "[FileService]") {
    fs::path tmp = fs::temp_directory_path() / "empty_dir";
    fs::create_directories(tmp);
    FileService fsrv;
    auto files = fsrv.listFiles(tmp);
    REQUIRE(files.empty());
    fs::remove_all(tmp);
}

TEST_CASE("FileService listFiles on non-existent directory throws", "[FileService]") {
    FileService fsrv;
    REQUIRE_THROWS_AS(fsrv.listFiles("/this/path/does/not/exist"), std::runtime_error);
}

TEST_CASE("FileService listFiles finds all files recursively", "[FileService]") {
    fs::path tmp = fs::temp_directory_path() / "list_test";
    fs::create_directories(tmp);
    std::ofstream(tmp / "a.txt") << "a";
    fs::create_directories(tmp / "sub");
    std::ofstream(tmp / "sub/b.txt") << "b";
    std::ofstream(tmp / "sub/c.txt") << "c";
    FileService fsrv;
    auto files = fsrv.listFiles(tmp);
    REQUIRE(files.size() == 3);
    fs::remove_all(tmp);
}

TEST_CASE("FileService copyFile copies content", "[FileService]") {
    fs::path src = fs::temp_directory_path() / "copy_src.txt";
    fs::path dst = fs::temp_directory_path() / "copy_dst.txt";
    std::ofstream(src) << "Hello, world!";
    FileService fsrv;
    fsrv.copyFile(src, dst);
    REQUIRE(fs::exists(dst));
    REQUIRE(fs::file_size(dst) == 13);
    fs::remove(src);
    fs::remove(dst);
}

TEST_CASE("FileService copyFile throws if source cannot be opened", "[FileService]") {
    FileService fsrv;
    REQUIRE_THROWS_AS(fsrv.copyFile("/nonexistent", "/tmp/out"), std::runtime_error);
}