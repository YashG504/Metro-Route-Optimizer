@echo off
echo Compiling Metro Route Optimizer...
g++ -std=c++17 -Wall -Wextra -o metro_app.exe main.cpp graph.cpp ui.cpp

if %errorlevel% neq 0 (
    echo Compilation failed!
    pause
    exit /b 1
)

echo.
echo Compilation successful! Starting application...
echo.
metro_app.exe

pause
