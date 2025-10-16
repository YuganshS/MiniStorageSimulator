#pragma once

#include <string>
#include <fstream>
#include <memory>

class StorageEngine {
private:
    std::string disk_file_name;
    size_t disk_size_bytes;
    size_t block_size_bytes;
    size_t total_blocks;
    std::unique_ptr<std::fstream> disk_file;
    
    void addLatency() const;

public:
    StorageEngine(const std::string& filename, size_t disk_size_mb, size_t block_size_bytes);
    ~StorageEngine();
    
    bool readBlock(int block_number, char* buffer);
    bool writeBlock(int block_number, const char* data);
    
    bool setupDisk();
    size_t getTotalBlocks() const { return total_blocks; }
    size_t getBlockSize() const { return block_size_bytes; }
    size_t getDiskSize() const { return disk_size_bytes; }
    
    bool isValidBlock(int block_number) const;
};
