TEMPLATE = app
TARGET = ClipboardManager

QT += core gui widgets

CONFIG += c++17

# Platform-specific libraries
macx {
    LIBS += -framework Carbon
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
