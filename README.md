# Mini Storage Simulator

A C++ virtual file I/O and block cache system that simulates storage engine operations similar to NetApp's WAFL (Write Anywhere File Layout) and caching systems.

## Overview

This project demonstrates core systems programming concepts including:
- File I/O operations with simulated latency
- LRU (Least Recently Used) cache implementation
- Block-based storage layout
- Performance metrics tracking
- Thread-safe operations

## Architecture

```
MiniStorage/
├── main.cpp                  # Entry point with menu-driven interface
├── storage_engine.cpp/.h     # Handles read/write operations to disk file
├── block_cache.cpp/.h        # LRU cache implementation
├── metrics.cpp/.h            # Collects and displays I/O metrics
├── utils.cpp/.h              # Helper functions (timing, file ops)
├── CMakeLists.txt            # Build configuration
└── README.md                 # This file
```

## Features

### 1. Virtual Disk
- Creates a binary file (`virtual_disk.bin`) acting as storage
- Fixed size: 10 MB divided into 4 KB blocks (2,560 total blocks)
- Simulates realistic disk I/O latency (1-5ms)

### 2. Block-Level I/O
- `readBlock(int block_id, char* buffer)` - Read data from specific block
- `writeBlock(int block_id, const char* data)` - Write data to specific block
- Automatic block validation and error handling

### 3. Cache System
- In-memory LRU cache for recently accessed blocks
- Configurable cache size (default: 100 blocks)
- Thread-safe operations with mutex protection
- Cache hit/miss ratio tracking

### 4. Performance Metrics
- Total read/write operations
- Cache hit/miss statistics
- Average latency measurements
- Real-time performance monitoring

### 5. Interactive CLI
- Menu-driven interface
- Block read/write operations
- Real-time cache statistics
- Detailed performance metrics

## Building and Running

### Prerequisites
- C++17 compatible compiler (GCC, Clang, or MSVC)
- CMake 3.16 or later

### Build Instructions

```bash
# Clone or download the project
cd MiniStorageSimulator

# Create build directory
mkdir build
cd build

# Configure and build
cmake ..
cmake --build .

# Run the simulator
./bin/mini_storage_simulator
```

### Windows (Visual Studio)
```cmd
mkdir build
cd build
cmake .. -G "Visual Studio 16 2019"
cmake --build . --config Release
.\bin\Release\mini_storage_simulator.exe
```

## Usage Example

```
Mini Storage Engine v1.0
------------------------
Virtual Disk: 10 MB (2560 blocks)
Block Size: 4096 bytes
Cache Size: 100 blocks

[1] Write Block
[2] Read Block
[3] Show Cache Stats
[4] Show Detailed Metrics
[5] Exit

> 1
Enter Block ID: 5
Enter Data (max 4096 characters): HelloWorld
Block written successfully.

> 2
Enter Block ID: 5
Block data (from cache): HelloWorld

> 3
Cache Statistics:
  Cache Hits: 1
  Cache Misses: 0
  Hit Ratio: 100.0%
  Cached Blocks: 1/100
  Average Latency: 2.4ms

> 4
Detailed I/O Metrics:
  Total Reads: 0
  Total Writes: 1
  Cache Hits: 1
  Cache Misses: 0
  Hit Ratio: 100.0%
  Average Latency: 2.4ms
  Total I/O Operations: 1
```

## Configuration

### Key Parameters (in `main.cpp`):
- `DISK_SIZE_MB`: Virtual disk size in megabytes (default: 10)
- `BLOCK_SIZE`: Block size in bytes (default: 4096)
- `CACHE_SIZE`: Maximum cached blocks (default: 100)

### Latency Simulation:
- Random latency between 1-5ms per I/O operation
- Simulates realistic disk access patterns

## Technical Details

### LRU Cache Implementation
- Uses `std::list` for O(1) insertion/deletion
- `std::unordered_map` for O(1) lookup
- Thread-safe with `std::mutex` protection

### Storage Engine
- File-based virtual disk with fixed block layout
- Automatic disk initialization with zero-filled blocks
- Error handling for invalid block IDs and I/O failures

### Metrics System
- Atomic counters for thread-safe statistics
- Real-time latency tracking
- Comprehensive performance monitoring

## Learning Objectives

This project demonstrates:
1. **Systems Programming**: File I/O, memory management, data structures
2. **Performance Awareness**: Simulated I/O latency, caching strategies
3. **Algorithmic Thinking**: LRU cache implementation, block mapping
4. **C++ Best Practices**: RAII, smart pointers, STL containers
5. **Concurrency**: Thread-safe operations, atomic operations


## License

This project is for educational purposes. Feel free to use and modify for learning.

## Contributing

This is an educational project. Suggestions and improvements are welcome!

---

**Note**: This simulator is designed for educational purposes and demonstrates storage system concepts. It's not intended for production use or real data storage.
