#include "../include/ClipboardManager.h"
#include "../include/GlobalHotkey.h"
#include <QApplication>
#include <QTimer>
#include <QDebug>
#include <QUrl>

ClipboardManager::ClipboardManager(QObject *parent)
    : QObject(parent),
      clipboard(QApplication::clipboard()),
      settings("Xclipy", "Xclipy"),
      globalHotkeyManager(nullptr) {

    loadSettings();
    history = settings.value("history").toStringList();
    qDebug() << "Loaded history:" << history;

    // Initialize global hotkey system
    if (GlobalHotkey::isSupported()) {
        globalHotkeyManager = new GlobalHotkey(this);
        if (globalHotkeyEnabled) {
            registerGlobalHotkey();
        }
        connect(globalHotkeyManager, &GlobalHotkey::hotkeyPressed, 
                this, &ClipboardManager::onGlobalHotkeyPressed);
    } else {
        qWarning() << "Global hotkeys not supported on this platform";
    }

    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &ClipboardManager::checkClipboard);
    timer->start(500);
}

const QStringList& ClipboardManager::getHistory() const {
    return history;
}

void ClipboardManager::clearHistory() {
    history.clear();
    settings.setValue("history", history);
    emit historyChanged(history);
}

void ClipboardManager::removeFromHistory(const QString &text) {
    if (history.removeAll(text) > 0) {
        settings.setValue("history", history);
        emit historyChanged(history);
    }
}

void ClipboardManager::setClipboardText(const QString &text) {
    selfCopy = true;                 // mark as self-triggered
    clipboard->setText(text);        // copy to clipboard
    lastText = text;                 // update last seen
}

void ClipboardManager::setClipboardFiles(const QStringList &filePaths) {
    selfCopy = true;                 // mark as self-triggered
    
    QMimeData *mimeData = new QMimeData();
    mimeData->setUrls(QUrl::fromStringList(filePaths));
    clipboard->setMimeData(mimeData);
    
    lastFiles = filePaths;           // update last seen files
}

void ClipboardManager::checkClipboard() {
    QString text = clipboard->text();
    QStringList files;

    // Check for files/folders in clipboard
    const QMimeData *mimeData = clipboard->mimeData();
    if (mimeData && mimeData->hasUrls()) {
        QList<QUrl> urls = mimeData->urls();
        for (const QUrl &url : urls) {
            if (url.isLocalFile()) {
                files.append(url.toLocalFile());
            }
        }
    }

    if (selfCopy) {
        // Skip the next clipboard change since it came from us
        selfCopy = false;
        return;
    }

    // Handle text clipboard changes
    if (!text.isEmpty() && text != lastText && !history.contains(text)) {
        lastText = text;
        history.prepend(text);
        if (history.size() > maxHistorySize) history.removeLast();
        settings.setValue("history", history);
        emit historyChanged(history);
        qDebug() << "Clipboard changed (text):" << text;
    }

    // Handle file/folder clipboard changes
    if (!files.isEmpty() && files != lastFiles) {
        lastFiles = files;
        QString fileEntry = files.join("\n");
        if (!history.contains(fileEntry)) {
            history.prepend(fileEntry);
            if (history.size() > maxHistorySize) history.removeLast();
            settings.setValue("history", history);
            emit historyChanged(history);
            qDebug() << "Clipboard changed (files):" << files;
        }
    }
}

// Settings management methods
void ClipboardManager::setMaxHistorySize(int size) {
    if (size > 0 && size != maxHistorySize) {
        maxHistorySize = size;
        // Trim history if new size is smaller
        while (history.size() > maxHistorySize) {
            history.removeLast();
        }
        settings.setValue("history", history);
        emit historyChanged(history);
        saveSettings();
    }
}

int ClipboardManager::getMaxHistorySize() const {
    return maxHistorySize;
}

void ClipboardManager::setAutoStart(bool enabled) {
    autoStart = enabled;
    saveSettings();
}

bool ClipboardManager::getAutoStart() const {
    return autoStart;
}

void ClipboardManager::setShowTrayIcon(bool enabled) {
    showTrayIcon = enabled;
    saveSettings();
}

bool ClipboardManager::getShowTrayIcon() const {
    return showTrayIcon;
}

void ClipboardManager::loadSettings() {
    maxHistorySize = settings.value("maxHistorySize", 50).toInt();
    autoStart = settings.value("autoStart", false).toBool();
    showTrayIcon = settings.value("showTrayIcon", true).toBool();
    globalHotkey = QKeySequence(settings.value("globalHotkey", "Ctrl+Shift+V").toString());
    globalHotkeyEnabled = settings.value("globalHotkeyEnabled", true).toBool();
}

void ClipboardManager::saveSettings() {
    settings.setValue("maxHistorySize", maxHistorySize);
    settings.setValue("autoStart", autoStart);
    settings.setValue("showTrayIcon", showTrayIcon);
    settings.setValue("globalHotkey", globalHotkey.toString());
    settings.setValue("globalHotkeyEnabled", globalHotkeyEnabled);
    settings.sync();
}

// Global hotkey management methods
void ClipboardManager::setGlobalHotkey(const QKeySequence &keySequence) {
    if (globalHotkey != keySequence) {
        globalHotkey = keySequence;
        if (globalHotkeyEnabled && globalHotkeyManager) {
            registerGlobalHotkey();
        }
        saveSettings();
    }
}

QKeySequence ClipboardManager::getGlobalHotkey() const {
    return globalHotkey;
}

bool ClipboardManager::isGlobalHotkeyEnabled() const {
    return globalHotkeyEnabled;
}

void ClipboardManager::setGlobalHotkeyEnabled(bool enabled) {
    if (globalHotkeyEnabled != enabled) {
        globalHotkeyEnabled = enabled;
        if (globalHotkeyManager) {
            if (enabled) {
                registerGlobalHotkey();
            } else {
                globalHotkeyManager->unregisterHotkey(0);
            }
        }
        saveSettings();
    }
}

void ClipboardManager::registerGlobalHotkey() {
    if (globalHotkeyManager && globalHotkeyEnabled) {
        globalHotkeyManager->unregisterHotkey(0); // Unregister previous hotkey
        bool success = globalHotkeyManager->registerHotkey(globalHotkey, 0);
        if (success) {
            qDebug() << "Global hotkey registered:" << globalHotkey.toString();
        } else {
            qWarning() << "Failed to register global hotkey:" << globalHotkey.toString();
        }
    }
}

void ClipboardManager::onGlobalHotkeyPressed(int id) {
    Q_UNUSED(id)
    qDebug() << "Global hotkey pressed - toggling history window";
    emit toggleHistoryRequested();
}
