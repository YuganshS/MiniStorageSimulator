#pragma once

#include <chrono>
#include <string>
#include <vector>

class Utils {
public:
    // Time utilities
    static std::chrono::milliseconds getCurrentTime();
    static std::string formatDuration(std::chrono::milliseconds duration);
    
    // File utilities
    static bool fileExists(const std::string& filename);
    static size_t getFileSize(const std::string& filename);
    static bool createDirectory(const std::string& path);
    
    // String utilities
    static std::string trim(const std::string& str);
    static std::vector<std::string> split(const std::string& str, char delimiter);
    static std::string toLower(const std::string& str);
    static std::string toUpper(const std::string& str);
    
    // Number formatting
    static std::string formatBytes(size_t bytes);
    static std::string formatNumber(size_t number);
};
