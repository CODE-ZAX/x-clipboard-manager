# ClipboardManager

A modern, feature-rich clipboard management application built with Qt/C++ for macOS, Linux, and Windows.

## Features

- **Smart Clipboard Monitoring**: Automatically captures clipboard changes with duplicate prevention
- **Configurable History**: Adjustable history size (10-1000 items)
- **Search & Filter**: Real-time search through clipboard history
- **Context Menu**: Right-click actions for individual items
- **System Tray Integration**: Background operation with tray icon
- **Global Hotkey Support**: Press `Ctrl+Shift+V` (configurable) to toggle history window from anywhere
- **Persistent Storage**: Settings and history saved across sessions
- **Preferences Window**: Easy configuration of application settings

## Project Structure

```
ClipboardManager/
├── src/                    # Source files
│   ├── main.cpp           # Application entry point
│   ├── ClipboardManager.cpp
│   ├── HistoryWindow.cpp
│   └── PreferencesWindow.cpp
├── include/               # Header files
│   ├── ClipboardManager.h
│   ├── HistoryWindow.h
│   └── PreferencesWindow.h
├── build/                 # Build artifacts (generated)
├── resources/             # Application resources
│   ├── resources.qrc
│   ├── icons/
│   ├── styles/
│   └── translations/
├── scripts/               # Build and utility scripts
│   └── build.sh
├── docs/                  # Documentation
├── ClipboardManager.pro   # Qt project file
└── README.md             # This file
```

## Requirements

- Qt 5.12 or later
- C++17 compatible compiler
- CMake 3.16+ (optional, for alternative build)

## Building

### Quick Build
```bash
# Make build script executable
chmod +x scripts/build.sh

# Build the application
./scripts/build.sh
```

### Manual Build
```bash
# Generate Makefile
qmake ClipboardManager.pro

# Build
make

# Run
./build/ClipboardManager
```

## Usage

1. **Launch**: Run the application - it will start in the system tray
2. **Show History**: Click tray icon → "Show History" or press the global hotkey
3. **Copy Items**: Click or double-click any item in the history window
4. **Copy Files/Folders**: Right-click file entries → "Copy as Files" to copy as file paths
5. **Search**: Use the search box to filter clipboard history
6. **Remove Items**: Right-click → "Remove" or "Clear All"
7. **Settings**: Tray icon → "Preferences" to configure the application

## Configuration

### Settings
- **History Size**: Maximum number of clipboard items to remember (10-1000)
- **Auto Start**: Launch application automatically at login
- **Tray Icon**: Show/hide system tray icon
- **Global Hotkey**: Customize the keyboard shortcut to show history
- **Global Hotkey Enabled**: Toggle global hotkey functionality

### Keyboard Shortcuts
- `Ctrl+C` / `Cmd+C`: Copy selected text (standard)
- `Ctrl+V` / `Cmd+V`: Paste (standard)
- `Ctrl+Shift+V` / `Cmd+Shift+V`: Toggle clipboard history window (configurable)
- Tray icon context menu for application control

## Development

### Adding New Features
1. Add source files to `src/`
2. Add headers to `include/`
3. Update `ClipboardManager.pro` with new files
4. Update this README if needed

### Code Style
- Follow Qt coding conventions
- Use meaningful variable and function names
- Add comments for complex logic
- Keep functions focused and small

## License

This project is licensed under the MIT License - see the LICENSE file for details.

## Contributing

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/amazing-feature`)
3. Commit your changes (`git commit -m 'Add amazing feature'`)
4. Push to the branch (`git push origin feature/amazing-feature`)
5. Open a Pull Request

## Version History

- **v1.0.0**: Initial release with core clipboard management features
- **v1.1.0**: Added search functionality and improved UI
- **v1.2.0**: Added preferences window and settings management

## Support

For issues and feature requests, please use the GitHub issue tracker.
