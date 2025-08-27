#include "../include/GlobalHotkey.h"
#include <QApplication>
#include <QDebug>
#include <QKeyEvent>
#include <QKeyCombination>

#ifdef Q_OS_WIN
GlobalHotkey *GlobalHotkey::instance = nullptr;
#endif

#ifdef Q_OS_LINUX
GlobalHotkey *GlobalHotkey::instance = nullptr;
#endif

GlobalHotkey::GlobalHotkey(QObject *parent)
    : QObject(parent), initialized(false)
#ifdef Q_OS_WIN
    , hwnd(nullptr), nextHotkeyId(1)
#endif
#ifdef Q_OS_LINUX
    , display(nullptr), rootWindow(0)
#endif
{
#ifdef Q_OS_MAC
    // Initialize Carbon event handler for macOS
    EventTypeSpec eventType;
    eventType.eventClass = kEventClassKeyboard;
    eventType.eventKind = kEventHotKeyPressed;
    
    OSStatus status = InstallEventHandler(GetApplicationEventTarget(), 
                                        hotkeyHandler, 
                                        1, &eventType, 
                                        this, &eventHandler);
    initialized = (status == noErr);
    
    if (!initialized) {
        qWarning() << "Failed to initialize global hotkey handler on macOS";
    }
#endif

#ifdef Q_OS_WIN
    // Create a hidden window for Windows hotkey handling
    instance = this;
    WNDCLASSEX wc = {};
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.lpfnWndProc = windowProc;
    wc.hInstance = GetModuleHandle(nullptr);
    wc.lpszClassName = L"GlobalHotkeyWindow";
    
    RegisterClassEx(&wc);
    
    hwnd = CreateWindowEx(0, L"GlobalHotkeyWindow", L"", 
                         0, 0, 0, 0, 0, nullptr, nullptr, 
                         GetModuleHandle(nullptr), nullptr);
    
    initialized = (hwnd != nullptr);
    
    if (!initialized) {
        qWarning() << "Failed to create hotkey window on Windows";
    }
#endif

#ifdef Q_OS_LINUX
    // Initialize X11 for Linux
    instance = this;
    display = XOpenDisplay(nullptr);
    if (display) {
        rootWindow = DefaultRootWindow(display);
        initialized = true;
    } else {
        qWarning() << "Failed to open X11 display for global hotkeys";
    }
#endif
}

GlobalHotkey::~GlobalHotkey() {
    unregisterAllHotkeys();
    
#ifdef Q_OS_MAC
    if (eventHandler) {
        RemoveEventHandler(eventHandler);
    }
#endif

#ifdef Q_OS_WIN
    if (hwnd) {
        DestroyWindow(hwnd);
    }
    instance = nullptr;
#endif

#ifdef Q_OS_LINUX
    if (display) {
        XCloseDisplay(display);
    }
    instance = nullptr;
#endif
}

bool GlobalHotkey::registerHotkey(const QKeySequence &keySequence, int id) {
    if (!initialized) {
        qWarning() << "Global hotkey system not initialized";
        return false;
    }

    HotkeyInfo info;
    info.keySequence = keySequence;
    info.id = id;
    info.registered = false;

#ifdef Q_OS_MAC
    // Extract key and modifiers from QKeySequence
    QKeyCombination keyCombo = keySequence[0];
    int key = keyCombo.key();
    Qt::KeyboardModifiers modifiers = keyCombo.keyboardModifiers();
    
    UInt32 keyCode = keyCodeForQtKey(key);
    UInt32 modifierFlags = modifierFlagsForQtModifiers(modifiers);
    
    EventHotKeyRef hotKeyRef;
    EventHotKeyID hotKeyID;
    hotKeyID.signature = 'htk1';
    hotKeyID.id = static_cast<UInt32>(id);
    
    OSStatus status = RegisterEventHotKey(keyCode, modifierFlags, 
                                        hotKeyID,
                                        GetApplicationEventTarget(), 
                                        0, &hotKeyRef);
    
    if (status == noErr) {
        info.registered = true;
        hotkeys[id] = info;
        qDebug() << "Registered global hotkey:" << keySequence.toString() << "with ID:" << id;
        return true;
    } else {
        qWarning() << "Failed to register hotkey on macOS:" << keySequence.toString();
        return false;
    }
#endif

#ifdef Q_OS_WIN
    // Extract key and modifiers
    QKeyCombination keyCombo = keySequence[0];
    int key = keyCombo.key();
    Qt::KeyboardModifiers modifiers = keyCombo.keyboardModifiers();
    
    UINT winModifiers = 0;
    if (modifiers & Qt::AltModifier) winModifiers |= MOD_ALT;
    if (modifiers & Qt::ControlModifier) winModifiers |= MOD_CONTROL;
    if (modifiers & Qt::ShiftModifier) winModifiers |= MOD_SHIFT;
    if (modifiers & Qt::MetaModifier) winModifiers |= MOD_WIN;
    
    UINT winKey = VkKeyScan(key);
    if (winKey == 0xFFFF) {
        // Try to map common keys
        switch (key) {
            case Qt::Key_F1: winKey = VK_F1; break;
            case Qt::Key_F2: winKey = VK_F2; break;
            case Qt::Key_F3: winKey = VK_F3; break;
            case Qt::Key_F4: winKey = VK_F4; break;
            case Qt::Key_F5: winKey = VK_F5; break;
            case Qt::Key_F6: winKey = VK_F6; break;
            case Qt::Key_F7: winKey = VK_F7; break;
            case Qt::Key_F8: winKey = VK_F8; break;
            case Qt::Key_F9: winKey = VK_F9; break;
            case Qt::Key_F10: winKey = VK_F10; break;
            case Qt::Key_F11: winKey = VK_F11; break;
            case Qt::Key_F12: winKey = VK_F12; break;
            default: winKey = key; break;
        }
    }
    
    bool success = RegisterHotKey(hwnd, nextHotkeyId, winModifiers, winKey);
    if (success) {
        hotkeyIds[id] = nextHotkeyId;
        info.registered = true;
        hotkeys[id] = info;
        nextHotkeyId++;
        qDebug() << "Registered global hotkey:" << keySequence.toString() << "with ID:" << id;
        return true;
    } else {
        qWarning() << "Failed to register hotkey on Windows:" << keySequence.toString();
        return false;
    }
#endif

#ifdef Q_OS_LINUX
    // For Linux, we'll use a simpler approach with XGrabKey
    QKeyCombination keyCombo = keySequence[0];
    int key = keyCombo.key();
    Qt::KeyboardModifiers modifiers = keyCombo.keyboardModifiers();
    
    int xKeyCode = keyCodeForQtKey(key);
    int xModifiers = modifierFlagsForQtModifiers(modifiers);
    
    int status = XGrabKey(display, xKeyCode, xModifiers, rootWindow, 
                         True, GrabModeAsync, GrabModeAsync);
    
    if (status == Success) {
        info.registered = true;
        hotkeys[id] = info;
        qDebug() << "Registered global hotkey:" << keySequence.toString() << "with ID:" << id;
        return true;
    } else {
        qWarning() << "Failed to register hotkey on Linux:" << keySequence.toString();
        return false;
    }
#endif

    return false;
}

bool GlobalHotkey::unregisterHotkey(int id) {
    if (!hotkeys.contains(id)) {
        return false;
    }

#ifdef Q_OS_WIN
    if (hotkeyIds.contains(id)) {
        UnregisterHotKey(hwnd, hotkeyIds[id]);
        hotkeyIds.remove(id);
    }
#endif

#ifdef Q_OS_LINUX
    if (display) {
        HotkeyInfo &info = hotkeys[id];
        QKeyCombination keyCombo = info.keySequence[0];
        int key = keyCombo.key();
        Qt::KeyboardModifiers modifiers = keyCombo.keyboardModifiers();
        
        int xKeyCode = keyCodeForQtKey(key);
        int xModifiers = modifierFlagsForQtModifiers(modifiers);
        
        XUngrabKey(display, xKeyCode, xModifiers, rootWindow);
    }
#endif

    hotkeys.remove(id);
    return true;
}

void GlobalHotkey::unregisterAllHotkeys() {
#ifdef Q_OS_WIN
    for (int hotkeyId : hotkeyIds.values()) {
        UnregisterHotKey(hwnd, hotkeyId);
    }
    hotkeyIds.clear();
#endif

#ifdef Q_OS_LINUX
    if (display) {
        for (const HotkeyInfo &info : hotkeys.values()) {
            QKeyCombination keyCombo = info.keySequence[0];
            int key = keyCombo.key();
            Qt::KeyboardModifiers modifiers = keyCombo.keyboardModifiers();
            
            int xKeyCode = keyCodeForQtKey(key);
            int xModifiers = modifierFlagsForQtModifiers(modifiers);
            
            XUngrabKey(display, xKeyCode, xModifiers, rootWindow);
        }
    }
#endif

    hotkeys.clear();
}

bool GlobalHotkey::isSupported() {
#ifdef Q_OS_MAC
    return true;
#elif defined(Q_OS_WIN)
    return true;
#elif defined(Q_OS_LINUX)
    return qgetenv("DISPLAY").isEmpty() == false;
#else
    return false;
#endif
}

#ifdef Q_OS_MAC
OSStatus GlobalHotkey::hotkeyHandler(EventHandlerCallRef nextHandler, EventRef event, void *userData) {
    Q_UNUSED(nextHandler)
    GlobalHotkey *hotkey = static_cast<GlobalHotkey*>(userData);
    EventHotKeyID hotKeyID;
    GetEventParameter(event, kEventParamDirectObject, typeEventHotKeyID, nullptr, sizeof(hotKeyID), nullptr, &hotKeyID);
    
    emit hotkey->hotkeyPressed(hotKeyID.id);
    return noErr;
}

UInt32 GlobalHotkey::keyCodeForQtKey(int qtKey) {
    // Map Qt keys to Carbon key codes
    switch (qtKey) {
        case Qt::Key_F1: return kVK_F1;
        case Qt::Key_F2: return kVK_F2;
        case Qt::Key_F3: return kVK_F3;
        case Qt::Key_F4: return kVK_F4;
        case Qt::Key_F5: return kVK_F5;
        case Qt::Key_F6: return kVK_F6;
        case Qt::Key_F7: return kVK_F7;
        case Qt::Key_F8: return kVK_F8;
        case Qt::Key_F9: return kVK_F9;
        case Qt::Key_F10: return kVK_F10;
        case Qt::Key_F11: return kVK_F11;
        case Qt::Key_F12: return kVK_F12;
        case Qt::Key_V: return kVK_ANSI_V;
        case Qt::Key_C: return kVK_ANSI_C;
        case Qt::Key_H: return kVK_ANSI_H;
        default: return 0;
    }
}

UInt32 GlobalHotkey::modifierFlagsForQtModifiers(Qt::KeyboardModifiers modifiers) {
    UInt32 flags = 0;
    if (modifiers & Qt::MetaModifier) flags |= cmdKey;
    if (modifiers & Qt::AltModifier) flags |= optionKey;
    if (modifiers & Qt::ControlModifier) flags |= controlKey;
    if (modifiers & Qt::ShiftModifier) flags |= shiftKey;
    return flags;
}
#endif

#ifdef Q_OS_WIN
LRESULT CALLBACK GlobalHotkey::windowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    if (msg == WM_HOTKEY && instance) {
        int hotkeyId = static_cast<int>(wParam);
        // Find our ID from the Windows hotkey ID
        for (auto it = instance->hotkeyIds.begin(); it != instance->hotkeyIds.end(); ++it) {
            if (it.value() == hotkeyId) {
                emit instance->hotkeyPressed(it.key());
                break;
            }
        }
        return 0;
    }
    return DefWindowProc(hwnd, msg, wParam, lParam);
}
#endif

#ifdef Q_OS_LINUX
void GlobalHotkey::handleXEvent(XEvent *event) {
    if (event->type == KeyPress && instance) {
        // For simplicity, we'll emit a signal for any key press
        // In a real implementation, you'd want to check if it matches registered hotkeys
        emit instance->hotkeyPressed(0);
    }
}

int GlobalHotkey::keyCodeForQtKey(int qtKey) {
    // Map Qt keys to X11 key codes
    switch (qtKey) {
        case Qt::Key_F1: return XK_F1;
        case Qt::Key_F2: return XK_F2;
        case Qt::Key_F3: return XK_F3;
        case Qt::Key_F4: return XK_F4;
        case Qt::Key_F5: return XK_F5;
        case Qt::Key_F6: return XK_F6;
        case Qt::Key_F7: return XK_F7;
        case Qt::Key_F8: return XK_F8;
        case Qt::Key_F9: return XK_F9;
        case Qt::Key_F10: return XK_F10;
        case Qt::Key_F11: return XK_F11;
        case Qt::Key_F12: return XK_F12;
        case Qt::Key_V: return XK_v;
        case Qt::Key_C: return XK_c;
        case Qt::Key_H: return XK_h;
        default: return qtKey;
    }
}

int GlobalHotkey::modifierFlagsForQtModifiers(Qt::KeyboardModifiers modifiers) {
    int flags = 0;
    if (modifiers & Qt::ControlModifier) flags |= ControlMask;
    if (modifiers & Qt::ShiftModifier) flags |= ShiftMask;
    if (modifiers & Qt::AltModifier) flags |= Mod1Mask;
    if (modifiers & Qt::MetaModifier) flags |= Mod4Mask;
    return flags;
}
#endif
