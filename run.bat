@echo off
REM Quick Start Script for Metro Route Optimizer (Windows)

setlocal enabledelayedexpansion

echo.
echo ================================================================
echo           METRO ROUTE OPTIMIZER - QUICK START (Windows)        
echo ================================================================
echo.

where g++ >nul 2>nul
if %ERRORLEVEL% neq 0 (
    echo [X] g++ compiler not found!
    echo Install MinGW from: https://www.mingw-w64.org/
    pause
    exit /b 1
)

echo [OK] g++ compiler found
echo.

if not exist metro-app.cpp (
    echo [X] Error: metro-app.cpp not found!
    pause
    exit /b 1
)

echo [OK] metro-app.cpp found
echo.
echo [*] Compiling...

g++ -std=c++17 -O2 -Wall -Wextra metro-app.cpp -o metro-app.exe

if %ERRORLEVEL% neq 0 (
    echo [X] Compilation failed!
    pause
    exit /b 1
)

echo [OK] Compilation successful!
echo.
echo [*] Starting Metro Route Optimizer...
echo.
echo ================================================================
echo.

metro-app.exe

if %ERRORLEVEL% neq 0 (
    echo [X] Application crashed or returned an error.
) else (
    echo [OK] Application exited successfully
)

pause
