@echo off
REM Quick Start Script for Metro Route Optimizer (Windows)
REM Run this to compile and execute the application

setlocal enabledelayedexpansion

echo.
echo ╔════════════════════════════════════════════════════════════════╗
echo ║          METRO ROUTE OPTIMIZER - QUICK START (Windows)        ║
echo ╚════════════════════════════════════════════════════════════════╝
echo.

REM Check if g++ is installed
where g++ >nul 2>nul
if %ERRORLEVEL% neq 0 (
    echo ❌ g++ compiler not found!
    echo.
    echo Install MinGW from: https://www.mingw-w64.org/
    echo Or use Visual Studio Build Tools
    echo Or Install MSVC with: Visual Studio Community
    pause
    exit /b 1
)

echo ✓ g++ compiler found
echo.

REM Check if metro-app.cpp exists
if not exist metro-app.cpp (
    echo ❌ metro-app.cpp not found in current directory!
    echo Please run this script from the project directory.
    pause
    exit /b 1
)

echo ✓ metro-app.cpp found
echo.

REM Compilation
echo 📦 Compiling ^(this may take a few seconds^)...
g++ -std=c++17 -O2 -Wall -Wextra metro-app.cpp -o metro-app.exe

if %ERRORLEVEL% neq 0 (
    echo ❌ Compilation failed!
    pause
    exit /b 1
)

echo ✓ Compilation successful!
echo.

echo 📊 Binary information:
dir metro-app.exe
echo.

REM Execution
echo 🚀 Starting Metro Route Optimizer...
echo.
echo ════════════════════════════════════════════════════════════════
echo.

metro-app.exe

echo.
echo ════════════════════════════════════════════════════════════════
echo ✓ Application exited successfully
pause
