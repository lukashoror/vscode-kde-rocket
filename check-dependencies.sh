#!/bin/bash
# Check and install dependencies for VS Code Launcher

echo "==================================="
echo "Checking Dependencies"
echo "==================================="

# Detect package manager
if command -v pacman &> /dev/null; then
    PKG_MANAGER="pacman"
    INSTALL_CMD="sudo pacman -S --needed"
    PACKAGES="qt6-base qt6-declarative yaml-cpp cmake gcc"
elif command -v apt &> /dev/null; then
    PKG_MANAGER="apt"
    INSTALL_CMD="sudo apt install -y"
    PACKAGES="qt6-base-dev qt6-declarative-dev libyaml-cpp-dev cmake build-essential"
elif command -v dnf &> /dev/null; then
    PKG_MANAGER="dnf"
    INSTALL_CMD="sudo dnf install -y"
    PACKAGES="qt6-qtbase-devel qt6-qtdeclarative-devel yaml-cpp-devel cmake gcc-c++"
else
    echo "Error: Could not detect package manager (pacman, apt, or dnf)"
    exit 1
fi

echo "Detected package manager: $PKG_MANAGER"
echo ""

# Check for required commands
MISSING_DEPS=()

if ! command -v cmake &> /dev/null; then
    echo "❌ CMake not found"
    MISSING_DEPS+=("cmake")
else
    echo "✓ CMake found: $(cmake --version | head -n1)"
fi

if ! command -v qmake6 &> /dev/null && ! command -v qmake-qt6 &> /dev/null; then
    echo "❌ Qt6 not found"
    MISSING_DEPS+=("qt6")
else
    echo "✓ Qt6 found"
fi

if ! pkg-config --exists yaml-cpp &> /dev/null; then
    echo "❌ yaml-cpp not found"
    MISSING_DEPS+=("yaml-cpp")
else
    echo "✓ yaml-cpp found"
fi

if ! command -v g++ &> /dev/null; then
    echo "❌ C++ compiler not found"
    MISSING_DEPS+=("gcc")
else
    echo "✓ C++ compiler found: $(g++ --version | head -n1)"
fi

echo ""

if [ ${#MISSING_DEPS[@]} -eq 0 ]; then
    echo "✓ All dependencies are installed!"
    exit 0
fi

echo "Missing dependencies detected."
echo ""
echo "To install all required dependencies, run:"
echo ""
echo "    $INSTALL_CMD $PACKAGES"
echo ""

read -p "Install dependencies now? (y/N) " -n 1 -r
echo
if [[ $REPLY =~ ^[Yy]$ ]]; then
    echo "Installing dependencies..."
    $INSTALL_CMD $PACKAGES
    echo ""
    echo "✓ Dependencies installed successfully!"
else
    echo "Skipping installation. Please install dependencies manually."
    exit 1
fi
