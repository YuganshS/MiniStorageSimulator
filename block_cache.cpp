#include "block_cache.h"
#include <algorithm>

BlockCache::BlockCache(size_t max_blocks) : max_blocks(max_blocks) {
}

std::string BlockCache::get(int block_number) {
    std::lock_guard<std::mutex> lock(cache_lock);
    
    auto it = block_map.find(block_number);
    if (it != block_map.end()) {
        auto list_it = it->second;
        std::string data = list_it->second;
        
        recent_blocks.splice(recent_blocks.begin(), recent_blocks, list_it);
        
        stats.hits++;
        return data;
    }
    
    stats.misses++;
    return "";
}

void BlockCache::put(int block_number, const char* data) {
    std::lock_guard<std::mutex> lock(cache_lock);
    
    auto it = block_map.find(block_number);
    if (it != block_map.end()) {
        auto list_it = it->second;
        list_it->second = data;
        
        recent_blocks.splice(recent_blocks.begin(), recent_blocks, list_it);
    } else {
        if (recent_blocks.size() >= max_blocks) {
            removeOldest();
        }
        
        recent_blocks.emplace_front(block_number, data);
        block_map[block_number] = recent_blocks.begin();
        stats.cached_blocks = recent_blocks.size();
    }
}

void BlockCache::remove(int block_number) {
    std::lock_guard<std::mutex> lock(cache_lock);
    
    auto it = block_map.find(block_number);
    if (it != block_map.end()) {
        recent_blocks.erase(it->second);
        block_map.erase(it);
        stats.cached_blocks = recent_blocks.size();
    }
}

void BlockCache::clear() {
    std::lock_guard<std::mutex> lock(cache_lock);
    
    recent_blocks.clear();
    block_map.clear();
    stats.cached_blocks = 0;
}

CacheStats BlockCache::getStats() const {
    std::lock_guard<std::mutex> lock(cache_lock);
    return stats;
}

size_t BlockCache::size() const {
    std::lock_guard<std::mutex> lock(cache_lock);
    return recent_blocks.size();
}

bool BlockCache::contains(int block_number) const {
    std::lock_guard<std::mutex> lock(cache_lock);
    return block_map.find(block_number) != block_map.end();
}

void BlockCache::removeOldest() {
    if (!recent_blocks.empty()) {
        auto last = recent_blocks.back();
        block_map.erase(last.first);
        recent_blocks.pop_back();
        stats.cached_blocks = recent_blocks.size();
    }
}
