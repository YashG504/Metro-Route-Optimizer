#!/bin/bash
# Quick Start Script for Metro Route Optimizer
# Run this to compile and execute the application

set -e  # Exit on error

echo "╔════════════════════════════════════════════════════════════════╗"
echo "║          METRO ROUTE OPTIMIZER - QUICK START                  ║"
echo "╚════════════════════════════════════════════════════════════════╝"
echo ""

# Check if g++ is installed
if ! command -v g++ &> /dev/null; then
    echo "❌ g++ compiler not found!"
    echo ""
    echo "Install it with:"
    echo "  Ubuntu/Debian: sudo apt-get install g++"
    echo "  Mac: brew install gcc"
    echo "  Windows: Download MinGW from https://www.mingw-w64.org/"
    exit 1
fi

echo "✓ g++ compiler found: $(g++ --version | head -n1)"
echo ""

# Check if metro-app.cpp exists
if [ ! -f "metro-app.cpp" ]; then
    echo "❌ metro-app.cpp not found in current directory!"
    echo "Please run this script from the project directory."
    exit 1
fi

echo "✓ metro-app.cpp found"
echo ""

# Compilation
echo "📦 Compiling (this may take a few seconds)..."
g++ -std=c++17 -O2 -Wall -Wextra metro-app.cpp -o metro-app

if [ $? -eq 0 ]; then
    echo "✓ Compilation successful!"
    echo ""
    echo "📊 Binary information:"
    file metro-app
    ls -lh metro-app
    echo ""
else
    echo "❌ Compilation failed!"
    exit 1
fi

# Execution
echo "🚀 Starting Metro Route Optimizer..."
echo ""
echo "════════════════════════════════════════════════════════════════"
echo ""

./metro-app

echo ""
echo "════════════════════════════════════════════════════════════════"
echo "✓ Application exited successfully"
