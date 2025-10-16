#include "storage_engine.h"
#include "utils.h"
#include <iostream>
#include <cstring>
#include <thread>
#include <chrono>
#include <random>
#include <vector>

StorageEngine::StorageEngine(const std::string& filename, size_t disk_size_mb, size_t block_size_bytes)
    : disk_file_name(filename)
    , disk_size_bytes(disk_size_mb * 1024 * 1024)
    , block_size_bytes(block_size_bytes)
    , total_blocks(disk_size_bytes / block_size_bytes) {
    
    if (!setupDisk()) {
        throw std::runtime_error("Failed to setup disk");
    }
}

StorageEngine::~StorageEngine() {
    if (disk_file && disk_file->is_open()) {
        disk_file->close();
    }
}

bool StorageEngine::setupDisk() {
    std::ifstream check_file(disk_file_name, std::ios::binary);
    bool file_exists = check_file.good();
    check_file.close();
    
    if (!file_exists) {
        std::ofstream create_file(disk_file_name, std::ios::binary);
        if (!create_file.is_open()) {
            return false;
        }
        
        std::vector<char> empty_block(block_size_bytes, 0);
        for (size_t i = 0; i < total_blocks; ++i) {
            create_file.write(empty_block.data(), block_size_bytes);
            if (create_file.fail()) {
                return false;
            }
        }
        
        create_file.close();
    }
    
    disk_file = std::make_unique<std::fstream>(disk_file_name, std::ios::in | std::ios::out | std::ios::binary);
    if (!disk_file->is_open()) {
        return false;
    }
    
    return true;
}

bool StorageEngine::readBlock(int block_number, char* buffer) {
    if (!isValidBlock(block_number)) {
        return false;
    }
    
    addLatency();
    
    std::streampos position = static_cast<std::streampos>(block_number * block_size_bytes);
    disk_file->seekg(position);
    
    if (disk_file->fail()) {
        return false;
    }
    
    disk_file->read(buffer, block_size_bytes);
    if (disk_file->fail() && !disk_file->eof()) {
        return false;
    }
    
    buffer[block_size_bytes - 1] = '\0';
    
    return true;
}

bool StorageEngine::writeBlock(int block_number, const char* data) {
    if (!isValidBlock(block_number)) {
        return false;
    }
    
    addLatency();
    
    std::streampos position = static_cast<std::streampos>(block_number * block_size_bytes);
    disk_file->seekp(position);
    
    if (disk_file->fail()) {
        return false;
    }
    
    std::vector<char> buffer(block_size_bytes, 0);
    size_t data_len = std::strlen(data);
    size_t copy_len = std::min(data_len, block_size_bytes - 1);
    std::memcpy(buffer.data(), data, copy_len);
    
    disk_file->write(buffer.data(), block_size_bytes);
    if (disk_file->fail()) {
        return false;
    }
    
    disk_file->flush();
    
    return true;
}

bool StorageEngine::isValidBlock(int block_number) const {
    return block_number >= 0 && static_cast<size_t>(block_number) < total_blocks;
}

void StorageEngine::addLatency() const {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 5);
    
    int latency_ms = dis(gen);
    std::this_thread::sleep_for(std::chrono::milliseconds(latency_ms));
}
