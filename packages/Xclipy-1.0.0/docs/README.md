# Xclipy - Cross Platform Clipboard Manager

A powerful, cross-platform clipboard manager that helps you efficiently manage your clipboard history with global hotkeys and a modern interface.

## Features

- **Always on Top**: History window stays above all applications for easy access
- **Global Hotkeys**: Quick access to clipboard history from anywhere
- **Smart History**: Automatically tracks text and file clipboard changes
- **Search & Filter**: Quickly find items in your clipboard history
- **Cross-Platform**: Works on macOS, Windows, and Linux
- **System Tray**: Runs in the background with easy access

## Installation

### macOS

#### Homebrew (Recommended)

```bash
brew install xclipy
```

#### Manual Installation

1. Download the latest release from [GitHub Releases](https://github.com/yourusername/xclipy/releases)
2. Extract the DMG file
3. Drag Xclipy to your Applications folder
4. Grant accessibility permissions in System Preferences > Security & Privacy > Privacy > Accessibility

### Windows

1. Download the latest Windows installer
2. Run the installer and follow the setup wizard
3. Launch Xclipy from the Start menu

### Linux

```bash
# Ubuntu/Debian
sudo apt-get install qt6-base-dev
git clone https://github.com/yourusername/xclipy.git
cd xclipy
mkdir build && cd build
cmake ..
make
sudo make install
```

## Usage

1. **Launch Xclipy** - The app will run in the background
2. **Global Hotkey** - Press `Cmd+Shift+V` (macOS) or `Ctrl+Shift+V` (Windows/Linux) to show/hide history
3. **Copy Items** - Click on any item in the history to copy it to clipboard
4. **Search** - Use the search box to filter your clipboard history
5. **Context Menu** - Right-click items for additional options

## Permissions Required

### macOS

- **Accessibility**: Required for global hotkeys and clipboard monitoring
- **Apple Events**: Required for clipboard change detection

### Windows

- **Global Hotkeys**: Automatically granted

### Linux

- **X11**: Required for global hotkeys and clipboard access

## Building from Source

### Prerequisites

- Qt6 (Core, Gui, Widgets)
- C++17 compatible compiler
- CMake 3.16+ or qmake

### Build Steps

```bash
git clone https://github.com/yourusername/xclipy.git
cd xclipy

# Using qmake
qmake ClipboardManager.pro
make

# Using CMake
mkdir build && cd build
cmake ..
make
```

## Development

### Project Structure

```
xclipy/
├── src/                    # Source files
├── include/               # Header files
├── resources/             # Icons, styles, translations
├── scripts/               # Build and deployment scripts
├── Formula/               # Homebrew formula
└── docs/                  # Documentation
```

### Contributing

1. Fork the repository
2. Create a feature branch
3. Make your changes
4. Submit a pull request

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Support

- **Issues**: [GitHub Issues](https://github.com/yourusername/xclipy/issues)
- **Discussions**: [GitHub Discussions](https://github.com/yourusername/xclipy/discussions)
- **Wiki**: [GitHub Wiki](https://github.com/yourusername/xclipy/wiki)

## Roadmap

- [ ] Cloud sync support
- [ ] Advanced formatting options
- [ ] Plugin system
- [ ] Mobile companion app
- [ ] API for third-party integrations
