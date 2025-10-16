cmake_minimum_required(VERSION 3.16)
project(MiniStorageSimulator VERSION 1.0.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set compiler flags
if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic")
endif()

# Add executable
add_executable(mini_storage_simulator
    main.cpp
    storage_engine.cpp
    block_cache.cpp
    metrics.cpp
    utils.cpp
)

# Include directories
target_include_directories(mini_storage_simulator PRIVATE .)

# Link libraries
if(WIN32)
    # Windows-specific libraries if needed
else()
    # Linux/Unix-specific libraries if needed
endif()

# Set output directory
set_target_properties(mini_storage_simulator PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

# Installation
install(TARGETS mini_storage_simulator
    RUNTIME DESTINATION bin
)

# Print build information
message(STATUS "Mini Storage Simulator Configuration:")
message(STATUS "  Version: ${PROJECT_VERSION}")
message(STATUS "  C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "  Build Type: ${CMAKE_BUILD_TYPE}")
message(STATUS "  Install Prefix: ${CMAKE_INSTALL_PREFIX}")
