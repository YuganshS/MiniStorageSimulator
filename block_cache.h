#pragma once

#include <unordered_map>
#include <list>
#include <string>
#include <mutex>

struct CacheStats {
    size_t hits = 0;
    size_t misses = 0;
    size_t cached_blocks = 0;
    
    double getHitRatio() const {
        size_t total = hits + misses;
        return total > 0 ? (static_cast<double>(hits) / total) * 100.0 : 0.0;
    }
};

class BlockCache {
private:
    size_t max_blocks;
    std::list<std::pair<int, std::string>> recent_blocks;  // Most recent at front
    std::unordered_map<int, std::list<std::pair<int, std::string>>::iterator> block_map;
    mutable std::mutex cache_lock;
    
    CacheStats stats;

public:
    explicit BlockCache(size_t max_blocks);
    
    std::string get(int block_number);
    void put(int block_number, const char* data);
    void remove(int block_number);
    void clear();
    
    CacheStats getStats() const;
    size_t size() const;
    bool contains(int block_number) const;
    
private:
    void removeOldest();
};
