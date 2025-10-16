#include "metrics.h"
#include <algorithm>

void Metrics::recordRead(std::chrono::milliseconds latency) {
    std::lock_guard<std::mutex> lock(update_mutex);
    
    data.total_reads++;
    data.total_operations++;
    data.total_latency_ms += static_cast<double>(latency.count());
    updateAverageLatency();
}

void Metrics::recordWrite(std::chrono::milliseconds latency) {
    std::lock_guard<std::mutex> lock(update_mutex);
    
    data.total_writes++;
    data.total_operations++;
    data.total_latency_ms += static_cast<double>(latency.count());
    updateAverageLatency();
}

void Metrics::recordCacheHit(std::chrono::milliseconds latency) {
    std::lock_guard<std::mutex> lock(update_mutex);
    
    data.cache_hits++;
    data.total_latency_ms += static_cast<double>(latency.count());
    updateAverageLatency();
}

void Metrics::recordCacheMiss(std::chrono::milliseconds latency) {
    std::lock_guard<std::mutex> lock(update_mutex);
    
    data.cache_misses++;
    data.total_latency_ms += static_cast<double>(latency.count());
    updateAverageLatency();
}

MetricsData Metrics::getMetrics() const {
    std::lock_guard<std::mutex> lock(update_mutex);
    
    MetricsData result;
    result.total_reads = data.total_reads;
    result.total_writes = data.total_writes;
    result.cache_hits = data.cache_hits;
    result.cache_misses = data.cache_misses;
    result.total_latency_ms = data.total_latency_ms;
    result.total_operations = data.total_operations;
    result.avg_latency_ms = data.avg_latency_ms;
    return result;
}

void Metrics::reset() {
    std::lock_guard<std::mutex> lock(update_mutex);
    
    data.total_reads = 0;
    data.total_writes = 0;
    data.cache_hits = 0;
    data.cache_misses = 0;
    data.total_latency_ms = 0.0;
    data.total_operations = 0;
    data.avg_latency_ms = 0.0;
}

void Metrics::updateAverageLatency() {
    size_t total_ops = data.total_operations;
    if (total_ops > 0) {
        data.avg_latency_ms = data.total_latency_ms / static_cast<double>(total_ops);
    }
}
