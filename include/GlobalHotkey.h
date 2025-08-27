#pragma once
#include <QObject>
#include <QKeySequence>
#include <QHash>

#ifdef Q_OS_MAC
#include <Carbon/Carbon.h>
#endif

#ifdef Q_OS_WIN
#include <windows.h>
#endif

#ifdef Q_OS_LINUX
#include <X11/Xlib.h>
#include <X11/keysym.h>
#endif

class GlobalHotkey : public QObject {
    Q_OBJECT

public:
    explicit GlobalHotkey(QObject *parent = nullptr);
    ~GlobalHotkey();

    // Register a global hotkey
    bool registerHotkey(const QKeySequence &keySequence, int id = 0);
    
    // Unregister a hotkey
    bool unregisterHotkey(int id = 0);
    
    // Unregister all hotkeys
    void unregisterAllHotkeys();
    
    // Check if hotkey is supported on this platform
    static bool isSupported();

signals:
    void hotkeyPressed(int id);

private:
    struct HotkeyInfo {
        QKeySequence keySequence;
        int id;
        bool registered;
    };

    QHash<int, HotkeyInfo> hotkeys;
    bool initialized;

#ifdef Q_OS_MAC
    EventHandlerRef eventHandler;
    static OSStatus hotkeyHandler(EventHandlerCallRef nextHandler, EventRef event, void *userData);
    UInt32 keyCodeForQtKey(int qtKey);
    UInt32 modifierFlagsForQtModifiers(Qt::KeyboardModifiers modifiers);
#endif

#ifdef Q_OS_WIN
    static LRESULT CALLBACK windowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
    HWND hwnd;
    static GlobalHotkey *instance;
    int nextHotkeyId;
    QHash<int, int> hotkeyIds; // Maps our ID to Windows hotkey ID
#endif

#ifdef Q_OS_LINUX
    Display *display;
    Window rootWindow;
    static GlobalHotkey *instance;
    static void handleXEvent(XEvent *event);
    int keyCodeForQtKey(int qtKey);
    int modifierFlagsForQtModifiers(Qt::KeyboardModifiers modifiers);
#endif
};
