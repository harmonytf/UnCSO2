#pragma once

#include <stdint.h>
#include <filesystem>
#include <span>
#include <vector>

namespace fs = std::filesystem;

std::pair<bool, std::vector<std::uint8_t>> ReadFileToBuffer(
    const fs::path& filePath, uint64_t readLength = 0 );
bool WriteBufferToFile( const fs::path& filePath,
                        std::span<std::uint8_t> buff );

bool CreateDirIfUnexisting( const fs::path& newDirPath ) noexcept;
