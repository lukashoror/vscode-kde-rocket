#!/bin/bash
# Deploy script for VS Code Launcher

set -e  # Exit on error

echo "==================================="
echo "Deploying VS Code Launcher"
echo "==================================="

# Check if build exists
if [ ! -d "build" ]; then
    echo "Error: Build directory not found!"
    echo "Please run ./build.sh first"
    exit 1
fi

cd build

# Install to ~/.local
echo "Installing to ~/.local..."
make install

# Install example configuration
echo "Installing configuration..."
make install-config

# Make sure ~/.local/bin is in PATH
if [[ ! ":$PATH:" == *":$HOME/.local/bin:"* ]]; then
    echo ""
    echo "WARNING: ~/.local/bin is not in your PATH"
    echo "Add this line to your ~/.bashrc or ~/.zshrc:"
    echo "    export PATH=\"\$HOME/.local/bin:\$PATH\""
fi

echo ""
echo "==================================="
echo "Deployment completed!"
echo "==================================="
echo ""
echo "To start the application:"
echo "    vscode-launcher"
echo ""
echo "To configure projects:"
echo "    1. Start the application"
echo "    2. Click the system tray icon"
echo "    3. Select 'Settings'"
echo ""
echo "Configuration file:"
echo "    ~/.config/vscode-launcher/config.yaml"
echo ""
echo "To enable autostart:"
echo "    cp ~/.local/share/applications/vscode-launcher.desktop ~/.config/autostart/"
