#include "utils.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <filesystem>
#include <locale>

std::chrono::milliseconds Utils::getCurrentTime() {
    return std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::high_resolution_clock::now().time_since_epoch()
    );
}

std::string Utils::formatDuration(std::chrono::milliseconds duration) {
    auto ms = duration.count();
    if (ms < 1000) {
        return std::to_string(ms) + "ms";
    } else if (ms < 60000) {
        return std::to_string(ms / 1000) + "s " + std::to_string(ms % 1000) + "ms";
    } else {
        auto minutes = ms / 60000;
        auto seconds = (ms % 60000) / 1000;
        return std::to_string(minutes) + "m " + std::to_string(seconds) + "s";
    }
}

bool Utils::fileExists(const std::string& filename) {
    std::ifstream file(filename);
    return file.good();
}

size_t Utils::getFileSize(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary | std::ios::ate);
    if (!file.is_open()) {
        return 0;
    }
    return static_cast<size_t>(file.tellg());
}

bool Utils::createDirectory(const std::string& path) {
    try {
        return std::filesystem::create_directories(path);
    } catch (const std::exception&) {
        return false;
    }
}

std::string Utils::trim(const std::string& str) {
    size_t first = str.find_first_not_of(' ');
    if (first == std::string::npos) {
        return "";
    }
    size_t last = str.find_last_not_of(' ');
    return str.substr(first, (last - first + 1));
}

std::vector<std::string> Utils::split(const std::string& str, char delimiter) {
    std::vector<std::string> tokens;
    std::stringstream ss(str);
    std::string token;
    
    while (std::getline(ss, token, delimiter)) {
        tokens.push_back(token);
    }
    
    return tokens;
}

std::string Utils::toLower(const std::string& str) {
    std::string result = str;
    std::transform(result.begin(), result.end(), result.begin(), ::tolower);
    return result;
}

std::string Utils::toUpper(const std::string& str) {
    std::string result = str;
    std::transform(result.begin(), result.end(), result.begin(), ::toupper);
    return result;
}

std::string Utils::formatBytes(size_t bytes) {
    const char* units[] = {"B", "KB", "MB", "GB", "TB"};
    int unit = 0;
    double size = static_cast<double>(bytes);
    
    while (size >= 1024.0 && unit < 4) {
        size /= 1024.0;
        unit++;
    }
    
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2) << size << " " << units[unit];
    return oss.str();
}

std::string Utils::formatNumber(size_t number) {
    std::ostringstream oss;
    oss.imbue(std::locale(""));
    oss << number;
    return oss.str();
}
