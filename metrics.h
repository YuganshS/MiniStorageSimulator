#pragma once

#include <chrono>
#include <atomic>
#include <mutex>

struct MetricsData {
    size_t total_reads = 0;
    size_t total_writes = 0;
    size_t cache_hits = 0;
    size_t cache_misses = 0;
    double total_latency_ms = 0.0;
    size_t total_operations = 0;
    
    double avg_latency_ms = 0.0;
    
    double getHitRatio() const {
        size_t total = cache_hits + cache_misses;
        return total > 0 ? (static_cast<double>(cache_hits) / total) * 100.0 : 0.0;
    }
    
    size_t getTotalOperations() const {
        return total_operations;
    }
};

class Metrics {
private:
    MetricsData data;
    mutable std::mutex update_mutex;

public:
    Metrics() = default;
    
    // Record operations
    void recordRead(std::chrono::milliseconds latency);
    void recordWrite(std::chrono::milliseconds latency);
    void recordCacheHit(std::chrono::milliseconds latency);
    void recordCacheMiss(std::chrono::milliseconds latency);
    
    // Get current metrics
    MetricsData getMetrics() const;
    
    // Reset metrics
    void reset();
    
private:
    void updateAverageLatency();
};
