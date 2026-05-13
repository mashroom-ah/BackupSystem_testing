#include <catch2/catch_test_macros.hpp>
#include "infrastructure/DiffService.h"
#include "infrastructure/HashService.h"
#include <fstream>

TEST_CASE("DiffService with empty hashes marks all as changed", "[DiffService]") {
    std::vector<std::filesystem::path> files = {"a.txt", "b.txt", "c.txt"};
    std::unordered_map<std::string, std::string> hashes;
    DiffService ds;
    auto changed = ds.getChangedFiles(files, hashes);
    REQUIRE(changed.size() == 3);
    REQUIRE(hashes.size() == 3);
}

TEST_CASE("DiffService with matching hashes marks none changed", "[DiffService]") {
    auto tmp = std::filesystem::temp_directory_path() / "diff_match.txt";
    std::ofstream(tmp) << "same";
    HashService hs;
    std::string hash = hs.calculateHash(tmp);
    std::vector<std::filesystem::path> files = {tmp};
    std::unordered_map<std::string, std::string> hashes = {{tmp.string(), hash}};
    DiffService ds;
    auto changed = ds.getChangedFiles(files, hashes);
    REQUIRE(changed.empty());
    REQUIRE(hashes[tmp.string()] == hash);
    std::filesystem::remove(tmp);
}

TEST_CASE("DiffService detects changed file content", "[DiffService]") {
    auto tmp = std::filesystem::temp_directory_path() / "diff_change.txt";
    std::ofstream(tmp) << "v1";
    HashService hs;
    std::string oldHash = hs.calculateHash(tmp);
    std::vector<std::filesystem::path> files = {tmp};
    std::unordered_map<std::string, std::string> hashes = {{tmp.string(), oldHash}};
    std::ofstream(tmp, std::ios::trunc) << "v2";
    DiffService ds;
    auto changed = ds.getChangedFiles(files, hashes);
    REQUIRE(changed.size() == 1);
    REQUIRE(hashes[tmp.string()] != oldHash);
    std::filesystem::remove(tmp);
}

TEST_CASE("DiffService handles new file not in hashes", "[DiffService]") {
    auto tmp = std::filesystem::temp_directory_path() / "newfile.txt";
    std::ofstream(tmp) << "new";
    std::vector<std::filesystem::path> files = {tmp};
    std::unordered_map<std::string, std::string> hashes;
    DiffService ds;
    auto changed = ds.getChangedFiles(files, hashes);
    REQUIRE(changed.size() == 1);
    REQUIRE(hashes.find(tmp.string()) != hashes.end());
    std::filesystem::remove(tmp);
}

TEST_CASE("DiffService updates hash for changed files", "[DiffService]") {
    auto tmp = std::filesystem::temp_directory_path() / "update_hash.txt";
    std::ofstream(tmp) << "initial";
    HashService hs;
    std::string h1 = hs.calculateHash(tmp);
    std::vector<std::filesystem::path> files = {tmp};
    std::unordered_map<std::string, std::string> hashes;
    DiffService ds;
    ds.getChangedFiles(files, hashes); // first time
    REQUIRE(hashes[tmp.string()] == h1);
    // change file
    std::ofstream(tmp, std::ios::trunc) << "changed";
    ds.getChangedFiles(files, hashes);
    std::string h2 = hs.calculateHash(tmp);
    REQUIRE(hashes[tmp.string()] == h2);
    REQUIRE(h2 != h1);
    std::filesystem::remove(tmp);
}

TEST_CASE("DiffService with multiple files some changed", "[DiffService]") {
    auto f1 = std::filesystem::temp_directory_path() / "f1.txt";
    auto f2 = std::filesystem::temp_directory_path() / "f2.txt";
    std::ofstream(f1) << "same";
    std::ofstream(f2) << "same";
    HashService hs;
    std::string h = hs.calculateHash(f1);
    std::vector<std::filesystem::path> files = {f1, f2};
    std::unordered_map<std::string, std::string> hashes = {{f1.string(), h}, {f2.string(), h}};
    // change f2
    std::ofstream(f2, std::ios::trunc) << "different";
    DiffService ds;
    auto changed = ds.getChangedFiles(files, hashes);
    REQUIRE(changed.size() == 1);
    REQUIRE(changed[0] == f2);
    std::filesystem::remove(f1);
    std::filesystem::remove(f2);
}