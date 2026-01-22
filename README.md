# VS Code Launcher

A KDE/Qt-based system tray application for quickly launching VS Code projects organized into groups.

## Features

- 🚀 Launch VS Code projects from system tray menu
- 📁 Organize projects into groups
- ⚙️ Easy YAML configuration
- 🔄 Auto-reload configuration after editing
- 🎨 Native KDE integration
- 📝 Settings button to edit configuration

## Dependencies

### Required packages:
- Qt6 (Core, Gui, Qml, Quick)
- yaml-cpp
- CMake (>= 3.16)
- C++ compiler with C++17 support

### Install on Arch/Manjaro:
```bash
sudo pacman -S qt6-base qt6-declarative yaml-cpp cmake gcc
```

### Install on Ubuntu/Debian:
```bash
sudo apt install qt6-base-dev qt6-declarative-dev libyaml-cpp-dev cmake build-essential
```

### Install on Fedora:
```bash
sudo dnf install qt6-qtbase-devel qt6-qtdeclarative-devel yaml-cpp-devel cmake gcc-c++
```

## Building

```bash
# Clone or navigate to the project directory
cd vscode-launcher

# Create build directory
mkdir build
cd build

# Configure with CMake
cmake ..

# Build
make

# Install system-wide (requires sudo)
sudo make install

# Or install to ~/.local (no sudo required)
cmake -DCMAKE_INSTALL_PREFIX=~/.local ..
make
make install

# Install example configuration
make install-config
```

## Configuration

The configuration file is located at: `~/.config/vscode-launcher/config.yaml`

### Example configuration:

```yaml
groups:
  - name: Personal Projects
    projects:
      - name: VS Code Launcher
        path: /home/user/Projects/vscode-launcher
      - name: My Website
        path: /home/user/Projects/website

  - name: Work Projects
    projects:
      - name: Backend API
        path: /home/user/Work/backend
      - name: Frontend App
        path: /home/user/Work/frontend

# Optional: Specify custom editor command for config editing
# editor_command: kate

# Optional: Specify custom VS Code command
# vscode_command: code
```

### Configuration Options:

- `groups`: List of project groups
  - `name`: Display name of the group
  - `projects`: List of projects in this group
    - `name`: Display name of the project
    - `path`: Absolute path to the project directory
- `editor_command`: (Optional) Command to open config file (default: auto-detected)
- `vscode_command`: (Optional) VS Code command (default: `code`)

## Usage

1. **Start the application:**
   ```bash
   vscode-launcher
   ```

2. **System Tray:**
   - Click the system tray icon to open the menu
   - Select a project from the grouped menu to open it in VS Code

3. **Edit Configuration:**
   - Click "Settings" in the tray menu
   - Edit the YAML file
   - Save and close the editor
   - Configuration will be automatically reloaded

4. **Manual Reload:**
   - Click "Reload Config" to manually refresh the project list

5. **Quit:**
   - Click "Quit" to exit the application

## Auto-start on Login

To start the application automatically on login:

1. Copy the desktop file to autostart:
   ```bash
   mkdir -p ~/.config/autostart
   cp ~/.local/share/applications/vscode-launcher.desktop ~/.config/autostart/
   ```

2. Or add it through KDE System Settings:
   - System Settings → Startup and Shutdown → Autostart
   - Click "Add..." → "Add Application..."
   - Search for "VS Code Launcher"

## Development

### Project Structure:
```
vscode-launcher/
├── CMakeLists.txt          # Build configuration
├── src/
│   ├── main.cpp            # Application entry point
│   ├── ProjectManager.h/cpp # YAML config management
│   └── LauncherService.h/cpp # VS Code launching
├── qml/
│   └── main.qml            # System tray UI
├── resources.qrc           # Qt resource file
├── icon.png                # Application icon
├── config.yaml.example     # Example configuration
└── vscode-launcher.desktop # Desktop entry
```

### Building for Development:
```bash
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Debug ..
make
./vscode-launcher
```

## Troubleshooting

**VS Code doesn't open:**
- Verify VS Code is installed: `which code`
- Specify custom command in config: `vscode_command: /path/to/code`

**Config editor doesn't open:**
- Specify custom editor in config: `editor_command: kate`
- Install a text editor: `sudo pacman -S kate`

**System tray icon doesn't appear:**
- Ensure system tray is enabled in KDE settings
- Try restarting the application

**Config not reloading:**
- Click "Reload Config" manually
- Check file permissions: `ls -l ~/.config/vscode-launcher/config.yaml`

## Uninstall

```bash
# If installed system-wide
sudo rm /usr/local/bin/vscode-launcher
sudo rm /usr/local/share/applications/vscode-launcher.desktop

# If installed to ~/.local
rm ~/.local/bin/vscode-launcher
rm ~/.local/share/applications/vscode-launcher.desktop

# Remove configuration
rm -rf ~/.config/vscode-launcher

# Remove from autostart
rm ~/.config/autostart/vscode-launcher.desktop
```

## License

MIT License - feel free to use and modify as needed.

## Contributing

Contributions are welcome! Feel free to open issues or submit pull requests.
