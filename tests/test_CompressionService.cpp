#include <catch2/catch_test_macros.hpp>
#include "infrastructure/CompressionService.h"
#include <fstream>
#include <filesystem>
#include <thread>
#include <chrono>

static void sync() {
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
}

TEST_CASE("CompressionService compress single file", "[Compression]") {
    auto dir = std::filesystem::temp_directory_path() / "compress_test";
    std::filesystem::create_directories(dir);
    std::ofstream(dir / "data.txt") << "hello";
    CompressionService cs;
    cs.compress(dir);
    sync();
    REQUIRE(std::filesystem::exists(dir / "data.txt.gz"));
    REQUIRE_FALSE(std::filesystem::exists(dir / "data.txt"));
    std::filesystem::remove_all(dir);
}

TEST_CASE("CompressionService compress multiple files", "[Compression]") {
    auto dir = std::filesystem::temp_directory_path() / "compress_multi";
    std::filesystem::create_directories(dir);
    std::ofstream(dir / "a.txt") << "a";
    std::ofstream(dir / "b.txt") << "b";
    CompressionService cs;
    cs.compress(dir);
    sync();
    REQUIRE(std::filesystem::exists(dir / "a.txt.gz"));
    REQUIRE(std::filesystem::exists(dir / "b.txt.gz"));
    REQUIRE_FALSE(std::filesystem::exists(dir / "a.txt"));
    REQUIRE_FALSE(std::filesystem::exists(dir / "b.txt"));
    std::filesystem::remove_all(dir);
}

TEST_CASE("CompressionService compress subdirectories", "[Compression]") {
    auto dir = std::filesystem::temp_directory_path() / "compress_sub";
    std::filesystem::create_directories(dir / "sub");
    std::ofstream(dir / "sub/file.txt") << "data";
    CompressionService cs;
    cs.compress(dir);
    sync();
    REQUIRE(std::filesystem::exists(dir / "sub/file.txt.gz"));
    std::filesystem::remove_all(dir);
}

TEST_CASE("CompressionService decompress restores original", "[Compression]") {
    auto srcDir = std::filesystem::temp_directory_path() / "decomp_src";
    auto dstDir = std::filesystem::temp_directory_path() / "decomp_dst";
    std::filesystem::create_directories(srcDir);
    std::ofstream(srcDir / "file.txt") << "content123";
    CompressionService cs;
    cs.compress(srcDir);
    sync();
    cs.decompress(srcDir, dstDir);
    sync();
    REQUIRE(std::filesystem::exists(dstDir / "file.txt"));
    std::ifstream in(dstDir / "file.txt");
    std::string s;
    in >> s;
    REQUIRE(s == "content123");
    in.close();
    std::filesystem::remove_all(srcDir);
    std::filesystem::remove_all(dstDir);
}

TEST_CASE("CompressionService decompress with multiple files", "[Compression]") {
    auto src = std::filesystem::temp_directory_path() / "decomp_multi_src";
    auto dst = std::filesystem::temp_directory_path() / "decomp_multi_dst";
    std::filesystem::create_directories(src);
    std::ofstream(src / "f1.txt") << "one";
    std::ofstream(src / "f2.txt") << "two";
    CompressionService cs;
    cs.compress(src);
    sync();
    cs.decompress(src, dst);
    sync();
    REQUIRE(std::filesystem::exists(dst / "f1.txt"));
    REQUIRE(std::filesystem::exists(dst / "f2.txt"));
    std::filesystem::remove_all(src);
    std::filesystem::remove_all(dst);
}