@echo off
echo Building Mini Storage Simulator...

if not exist build mkdir build
cd build

echo Configuring with CMake...
cmake .. -G "Visual Studio 16 2019" -A x64

if %ERRORLEVEL% neq 0 (
    echo CMake configuration failed!
    pause
    exit /b 1
)

echo Building project...
cmake --build . --config Release

if %ERRORLEVEL% neq 0 (
    echo Build failed!
    pause
    exit /b 1
)

echo Build successful!
echo Executable location: build\bin\Release\mini_storage_simulator.exe
pause
