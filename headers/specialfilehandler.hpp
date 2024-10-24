#pragma once

#include <cstdint>
#include <filesystem>
#include <string_view>
#include <span>

namespace fs = std::filesystem;

class SpecialFileHandler
{
public:
    SpecialFileHandler( std::span<std::uint8_t> fileData, fs::path filePath,
                        bool canDecrypt, bool canDecompress );
    ~SpecialFileHandler();

public:
    std::span<std::uint8_t> ProcessData();
    inline fs::path GetNewFilePath() const;

private:
    bool IsFileEncrypted() const;
    bool IsTextureCompressed() const;

    std::span<std::uint8_t> DecryptFile();
    std::span<std::uint8_t> DecompressTexture() const;

    static void FixDecryptedExtension( fs::path& filePath );

private:
    std::span<std::uint8_t> m_FileData;
    fs::path m_FilePath;

    const bool m_bAllowDecryption;
    const bool m_bAllowDecompression;

private:
    SpecialFileHandler() = delete;
    SpecialFileHandler& operator=( const SpecialFileHandler& ) = delete;
    SpecialFileHandler( const SpecialFileHandler& ) = delete;
};

inline fs::path SpecialFileHandler::GetNewFilePath() const
{
    return this->m_FilePath;
}
