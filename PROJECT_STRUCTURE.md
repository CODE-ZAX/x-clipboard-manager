# ClipboardManager Project Structure

This document describes the production-ready folder structure and organization of the ClipboardManager project.

## Directory Structure

```
ClipboardManager/
├── src/                           # Source files
│   ├── main.cpp                   # Application entry point
│   ├── ClipboardManager.cpp       # Core clipboard management logic
│   ├── HistoryWindow.cpp          # History window implementation
│   ├── PreferencesWindow.cpp      # Preferences dialog implementation
│   └── GlobalHotkey.cpp           # Global hotkey implementation
│
├── include/                       # Header files
│   ├── ClipboardManager.h         # Core manager header
│   ├── HistoryWindow.h            # History window header
│   ├── PreferencesWindow.h        # Preferences window header
│   └── GlobalHotkey.h             # Global hotkey header
│
├── build/                         # Build artifacts (generated)
│   ├── ClipboardManager.app/      # macOS application bundle
│   ├── *.o                        # Object files
│   ├── moc_*.cpp                  # Qt MOC generated files
│   ├── Makefile                   # Generated Makefile
│   └── .qmake.stash              # Qt build cache
│
├── resources/                     # Application resources
│   ├── resources.qrc             # Qt resource file
│   ├── icons/                    # Application icons
│   │   ├── clipboard.png
│   │   ├── settings.png
│   │   └── clear.png
│   ├── styles/                   # Stylesheets
│   │   └── main.qss
│   └── translations/             # Localization files
│       └── clipboardmanager_en.qm
│
├── scripts/                      # Build and utility scripts
│   ├── build.sh                  # Main build script
│   ├── dev.sh                    # Development utilities
│   ├── package.sh                # Packaging script
│   ├── test_hotkey.sh            # Global hotkey test script
│   └── test_features.sh          # Comprehensive feature test script
│
├── packages/                     # Distribution packages (generated)
│   └── ClipboardManager-1.2.0/   # Package contents
│
├── docs/                         # Documentation
│   └── (future documentation files)
│
├── .github/                      # GitHub configuration
│   └── workflows/
│       └── ci.yml                # CI/CD pipeline
│
├── ClipboardManager.pro          # Qt project file
├── CMakeLists.txt               # CMake build configuration
├── README.md                    # Project documentation
├── CONTRIBUTING.md              # Contribution guidelines
├── LICENSE                      # MIT License
├── .gitignore                   # Git ignore rules
└── PROJECT_STRUCTURE.md         # This file
```

## File Organization Principles

### 1. Separation of Concerns
- **Source files** (`src/`): Implementation files (.cpp)
- **Header files** (`include/`): Interface definitions (.h)
- **Resources** (`resources/`): Assets, styles, translations
- **Build artifacts** (`build/`): Generated files, not in version control

### 2. Build System Support
- **Qt/qmake**: Primary build system via `ClipboardManager.pro`
- **CMake**: Alternative build system via `CMakeLists.txt`
- **Cross-platform**: Supports macOS, Linux, and Windows

### 3. Development Workflow
- **Scripts** (`scripts/`): Automated build, test, and packaging
- **CI/CD** (`.github/workflows/`): Automated testing and deployment
- **Documentation**: Comprehensive guides and API docs

## Key Features

### Build System
- **Dual build support**: qmake and CMake
- **Resource management**: Qt resource system integration
- **Cross-platform**: macOS, Linux, Windows support
- **Optimized builds**: Release and debug configurations

### Development Tools
- **Build script**: `./scripts/build.sh` - One-command build
- **Development script**: `./scripts/dev.sh` - Development utilities
- **Packaging script**: `./scripts/package.sh` - Distribution packages
- **CI/CD pipeline**: Automated testing and releases

### Project Management
- **Version control**: Git with proper .gitignore
- **Documentation**: README, CONTRIBUTING, and API docs
- **Licensing**: MIT License for open source
- **Issue tracking**: GitHub Issues integration

## Build Instructions

### Quick Start
```bash
# Clone the repository
git clone <repository-url>
cd ClipboardManager

# Build the application
chmod +x scripts/*.sh
./scripts/build.sh

# Run the application
./build/ClipboardManager
```

### Development Workflow
```bash
# Development utilities
./scripts/dev.sh help          # Show available commands
./scripts/dev.sh build         # Build the application
./scripts/dev.sh run           # Build and run
./scripts/dev.sh clean         # Clean build artifacts
```

### Packaging
```bash
# Create distribution packages
./scripts/package.sh
```

## Repository Ready

The project is now ready for:

1. **Git Repository**: All files properly organized
2. **Version Control**: Comprehensive .gitignore
3. **CI/CD**: GitHub Actions workflow
4. **Documentation**: Complete README and guides
5. **Contributing**: CONTRIBUTING.md with guidelines
6. **Licensing**: MIT License included
7. **Packaging**: Automated distribution scripts

## Next Steps

1. **Initialize Git repository**:
   ```bash
   git init
   git add .
   git commit -m "Initial commit: ClipboardManager v1.2.0"
   ```

2. **Create GitHub repository** and push:
   ```bash
   git remote add origin <github-repo-url>
   git push -u origin main
   ```

3. **Set up CI/CD**: GitHub Actions will automatically run on push

4. **Create releases**: Use the packaging script for distribution

The project is now production-ready with a professional structure suitable for open-source distribution and collaborative development.
