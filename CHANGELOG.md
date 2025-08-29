# Changelog

All notable changes to Xclipy will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [1.1.0] - 2024-08-27

### ✨ Added

- **Enhanced UI with striped rows** - Better visual separation between clipboard items
- **Always-visible simple delete buttons** - Clean gray X buttons for each item (no hover effects)
- **Rich text formatting** - Multi-line clipboard items now display with bullet points
- **Smart text truncation** - Clipboard items limited to 4 lines with ellipsis ("...") indicator
- **Improved tray integration** - History window stays open when accessed via system tray
- **Clean interface** - No tooltips for simplified user experience
- **Visual hover effects** - Items highlight when hovered over for better interaction feedback
- **Copy notification popup** - "Item copied!" notification appears and fades out when clicking items

### 🔧 Improved

- **Better text rendering** - Uses QTextDocument for enhanced text display
- **Smarter window behavior** - Auto-hide only when copying from history list
- **Custom item delegate** - Professional list item rendering with simple controls
- **Dynamic item sizing** - List items resize based on content length
- **Simplified interface** - Removed tooltips for cleaner user experience

### 🎨 UI/UX Enhancements

- Alternating row colors for better readability
- Simple gray X delete buttons (no hover effects)
- Bullet points for multi-line clipboard content
- Text truncation with "..." for long content
- Improved visual hierarchy and spacing
- Clean interface without distracting tooltips
- Visual hover feedback with light highlight colors
- Smooth fade-out notification animation for copy feedback

### 🐛 Bug Fixes

- Fixed window hiding behavior when navigating clipboard history
- Improved text display for file paths and multi-line content
- Better handling of rich text content

## [1.0.0] - 2024-08-27

### 🎉 Initial Release

- **Cross-platform clipboard manager** for macOS, Linux, and Windows
- **Global hotkeys** for quick access to clipboard history
- **System tray integration** with contextual menu
- **Search and filter** functionality for clipboard history
- **File and text support** with smart detection
- **Persistent storage** of clipboard history
- **Always-on-top window** for easy access
- **Configurable settings** via preferences window
- **Professional branding** as "Xclipy - Cross Platform Clipboard"

### 🚀 Features

- Monitor clipboard changes automatically
- Store up to configurable number of clipboard items
- Support for both text and file clipboard content
- Right-click context menu for advanced operations
- Global hotkey support (Cmd+Shift+V on macOS)
- Background operation with system tray icon
- Modern Qt6-based interface

---

**Legend:**

- ✨ Added: New features
- 🔧 Improved: Enhancements to existing features
- 🎨 UI/UX: User interface and experience improvements
- 🐛 Bug Fixes: Bug fixes and stability improvements
- 🚀 Features: Major feature additions
- 🎉 Initial: First release features
