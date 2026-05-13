#include <catch2/catch_test_macros.hpp>
#include "infrastructure/HashService.h"
#include <fstream>

TEST_CASE("HashService calculateHash on empty file returns 0", "[HashService]") {
    auto tmp = std::filesystem::temp_directory_path() / "empty.txt";
    std::ofstream(tmp) << "";
    HashService hs;
    std::string hash = hs.calculateHash(tmp);
    REQUIRE_FALSE(hash.empty());
    REQUIRE(hash == "0");
    std::filesystem::remove(tmp);
}

TEST_CASE("HashService same content yields same hash", "[HashService]") {
    auto f1 = std::filesystem::temp_directory_path() / "f1.txt";
    auto f2 = std::filesystem::temp_directory_path() / "f2.txt";
    std::ofstream(f1) << "content";
    std::ofstream(f2) << "content";
    HashService hs;
    REQUIRE(hs.calculateHash(f1) == hs.calculateHash(f2));
    std::filesystem::remove(f1);
    std::filesystem::remove(f2);
}

TEST_CASE("HashService different content yields different hash", "[HashService]") {
    auto f1 = std::filesystem::temp_directory_path() / "f1.txt";
    auto f2 = std::filesystem::temp_directory_path() / "f2.txt";
    std::ofstream(f1) << "content1";
    std::ofstream(f2) << "content2";
    HashService hs;
    REQUIRE(hs.calculateHash(f1) != hs.calculateHash(f2));
    std::filesystem::remove(f1);
    std::filesystem::remove(f2);
}

TEST_CASE("HashService large file (1 MB) does not crash", "[HashService]") {
    auto tmp = std::filesystem::temp_directory_path() / "large.bin";
    std::ofstream ofs(tmp, std::ios::binary);
    for (int i = 0; i < 1024 * 1024; ++i) ofs << 'x';
    ofs.close();
    HashService hs;
    REQUIRE_NOTHROW(hs.calculateHash(tmp));
    std::filesystem::remove(tmp);
}

TEST_CASE("HashService binary file integrity", "[HashService]") {
    auto tmp = std::filesystem::temp_directory_path() / "binary.dat";
    std::ofstream ofs(tmp, std::ios::binary);
    unsigned char data[] = {0x00, 0xFF, 0x12, 0x34};
    ofs.write(reinterpret_cast<char*>(data), sizeof(data));
    ofs.close();
    HashService hs;
    std::string hash = hs.calculateHash(tmp);
    REQUIRE_FALSE(hash.empty());
    std::filesystem::remove(tmp);
}

TEST_CASE("HashService non-existent file returns empty string", "[HashService]") {
    HashService hs;
    REQUIRE(hs.calculateHash("/no/such/file") == "");
}