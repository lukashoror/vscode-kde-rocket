#!/bin/bash
# Install config only if it doesn't exist

CONFIG_DIR="$HOME/.config/vscode-launcher"
CONFIG_FILE="$CONFIG_DIR/config.yaml"

# Create directory if it doesn't exist
mkdir -p "$CONFIG_DIR"

# Copy config only if it doesn't exist
if [ ! -f "$CONFIG_FILE" ]; then
    cp "$1" "$CONFIG_FILE"
    echo "Config installed to $CONFIG_FILE"
else
    echo "Config already exists, skipping to preserve existing configuration"
fi
