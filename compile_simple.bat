@echo off
echo Compiling Mini Storage Simulator...

REM Try to find a C++ compiler
where g++ >nul 2>&1
if %ERRORLEVEL% equ 0 (
    echo Found g++ compiler
    g++ -std=c++17 -O2 -Wall -Wextra *.cpp -o mini_storage_simulator.exe
    goto :end
)

where clang++ >nul 2>&1
if %ERRORLEVEL% equ 0 (
    echo Found clang++ compiler
    clang++ -std=c++17 -O2 -Wall -Wextra *.cpp -o mini_storage_simulator.exe
    goto :end
)

where cl >nul 2>&1
if %ERRORLEVEL% equ 0 (
    echo Found MSVC compiler
    cl /std:c++17 /O2 /EHsc *.cpp /Fe:mini_storage_simulator.exe
    goto :end
)

echo No C++ compiler found!
echo Please install one of the following:
echo 1. MinGW-w64: https://www.mingw-w64.org/downloads/
echo 2. LLVM/Clang: https://releases.llvm.org/download.html
echo 3. Visual Studio Community: https://visualstudio.microsoft.com/vs/community/
goto :error

:end
if %ERRORLEVEL% equ 0 (
    echo Compilation successful!
    echo Executable: mini_storage_simulator.exe
    echo To run: .\mini_storage_simulator.exe
) else (
    echo Compilation failed!
    goto :error
)
goto :eof

:error
pause
exit /b 1
