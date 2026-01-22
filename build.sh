#!/bin/bash
# Build script for VS Code Launcher

set -e  # Exit on error

echo "==================================="
echo "Building VS Code Launcher"
echo "==================================="

# Clean previous build
if [ -d "build" ]; then
    echo "Cleaning previous build..."
    rm -rf build
fi

# Create build directory
mkdir -p build
cd build

# Configure with CMake
echo "Configuring with CMake..."
cmake -DCMAKE_BUILD_TYPE=Release \
      -DCMAKE_INSTALL_PREFIX=~/.local \
      ..

# Build
echo "Building..."
make -j$(nproc)

echo "==================================="
echo "Build completed successfully!"
echo "==================================="
echo "To install, run: ./deploy.sh"
