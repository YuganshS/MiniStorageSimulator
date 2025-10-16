#include <iostream>
#include <string>
#include <memory>
#include <iomanip>
#include "storage_engine.h"
#include "block_cache.h"
#include "metrics.h"
#include "utils.h"

class StorageSimulator {
private:
    std::unique_ptr<StorageEngine> disk;
    std::unique_ptr<BlockCache> memory_cache;
    std::unique_ptr<Metrics> stats;
    
    static constexpr size_t disk_size_mb = 10;
    static constexpr size_t block_size_bytes = 4096;
    static constexpr size_t max_cached_blocks = 100;

public:
    StorageSimulator() 
        : disk(std::make_unique<StorageEngine>("virtual_disk.bin", disk_size_mb, block_size_bytes))
        , memory_cache(std::make_unique<BlockCache>(max_cached_blocks))
        , stats(std::make_unique<Metrics>()) {
        
        std::cout << "Storage Simulator v1.0" << std::endl;
        std::cout << "Disk: " << disk_size_mb << "MB, Cache: " << max_cached_blocks << " blocks" << std::endl;
    }

    void run() {
        int choice;
        while (true) {
            showMenu();
            std::cout << "> ";
            std::cin >> choice;
            
            switch (choice) {
                case 1:
                    writeBlock();
                    break;
                case 2:
                    readBlock();
                    break;
                case 3:
                    showStats();
                    break;
                case 4:
                    std::cout << "Goodbye!" << std::endl;
                    return;
                default:
                    std::cout << "Invalid choice." << std::endl;
            }
        }
    }

private:
    void showMenu() {
        std::cout << "[1] Write Block" << std::endl;
        std::cout << "[2] Read Block" << std::endl;
        std::cout << "[3] Show Stats" << std::endl;
        std::cout << "[4] Exit" << std::endl;
    }

    void writeBlock() {
        int block_number;
        std::string user_data;
        
        std::cout << "Block ID: ";
        std::cin >> block_number;
        
        size_t total_blocks = disk_size_mb * 1024 * 1024 / block_size_bytes;
        if (block_number < 0 || block_number >= total_blocks) {
            std::cout << "Invalid block ID (0-" << (total_blocks - 1) << ")" << std::endl;
            return;
        }
        
        std::cout << "Data: ";
        std::cin.ignore();
        std::getline(std::cin, user_data);
        
        if (user_data.length() > block_size_bytes) {
            user_data = user_data.substr(0, block_size_bytes);
        }
        
        auto start = Utils::getCurrentTime();
        bool success = disk->writeBlock(block_number, user_data.c_str());
        auto end = Utils::getCurrentTime();
        
        if (success) {
            memory_cache->put(block_number, user_data.c_str());
            stats->recordWrite(end - start);
            std::cout << "Written." << std::endl;
        } else {
            std::cout << "Write failed." << std::endl;
        }
    }

    void readBlock() {
        int block_number;
        
        std::cout << "Block ID: ";
        std::cin >> block_number;
        
        size_t total_blocks = disk_size_mb * 1024 * 1024 / block_size_bytes;
        if (block_number < 0 || block_number >= total_blocks) {
            std::cout << "Invalid block ID (0-" << (total_blocks - 1) << ")" << std::endl;
            return;
        }
        
        auto start = Utils::getCurrentTime();
        
        // Try cache first
        std::string cached_data = memory_cache->get(block_number);
        if (!cached_data.empty()) {
            auto end = Utils::getCurrentTime();
            stats->recordCacheHit(end - start);
            std::cout << "Data: " << cached_data << std::endl;
            return;
        }
        
        // Read from disk
        char buffer[block_size_bytes];
        bool success = disk->readBlock(block_number, buffer);
        auto end = Utils::getCurrentTime();
        
        if (success) {
            std::string data(buffer);
            memory_cache->put(block_number, data.c_str());
            stats->recordCacheMiss(end - start);
            std::cout << "Data: " << data << std::endl;
        } else {
            std::cout << "Read failed." << std::endl;
        }
    }

    void showStats() {
        auto cache_stats = memory_cache->getStats();
        auto performance_data = stats->getMetrics();
        
        std::cout << "Cache: " << cache_stats.hits << " hits, " << cache_stats.misses << " misses" << std::endl;
        std::cout << "Hit rate: " << std::fixed << std::setprecision(1) 
                  << cache_stats.getHitRatio() << "%" << std::endl;
        std::cout << "Avg latency: " << std::fixed << std::setprecision(1) 
                  << performance_data.avg_latency_ms << "ms" << std::endl;
    }
};

int main() {
    try {
        StorageSimulator simulator;
        simulator.run();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}

