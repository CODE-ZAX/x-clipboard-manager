TEMPLATE = app
TARGET = Xclipy

# Application metadata
VERSION = 1.0.0
DEFINES += APP_VERSION=\\\"$$VERSION\\\"
DEFINES += APP_NAME=\\\"Xclipy\\\"
DEFINES += APP_ORGANIZATION=\\\"Xclipy\\\"
DEFINES += APP_DOMAIN=\\\"xclipy.com\\\"

QT += core gui widgets

CONFIG += c++17

# Platform-specific libraries
macx {
    LIBS += -framework Carbon
    
    # macOS bundle configuration
    ICON = resources/icons/clipboard.icns
    QMAKE_INFO_PLIST = Info.plist
    
    # Set bundle version
    QMAKE_BUNDLE_VERSION = 1.0.0
    QMAKE_BUNDLE_SHORT_VERSION_STRING = 1.0
    
    # Bundle identifier
    QMAKE_BUNDLE_IDENTIFIER = com.xclipy.app
}

win32 {
    # Windows doesn't need additional libraries for global hotkeys
}

unix:!macx {
    # Linux needs X11
    CONFIG += link_pkgconfig
    PKGCONFIG += x11
}

# Source files
SOURCES += \
    src/main.cpp \
    src/ClipboardManager.cpp \
    src/HistoryWindow.cpp \
    src/PreferencesWindow.cpp \
    src/GlobalHotkey.cpp

# Header files
HEADERS += \
    include/ClipboardManager.h \
    include/HistoryWindow.h \
    include/PreferencesWindow.h \
    include/GlobalHotkey.h

# Build directory
DESTDIR = build

# Object files directory
OBJECTS_DIR = build

# MOC files directory
MOC_DIR = build

# UI files directory (if any)
UI_DIR = build

# Resource files
RESOURCES += \
    resources/resources.qrc

# Default rules for deployment
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
