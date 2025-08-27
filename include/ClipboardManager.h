#pragma once
#include <QObject>
#include <QClipboard>
#include <QStringList>
#include <QSettings>
#include <QKeySequence>
#include <QMimeData>

class GlobalHotkey;

class ClipboardManager : public QObject {
    Q_OBJECT

public:
    explicit ClipboardManager(QObject *parent = nullptr);
    const QStringList& getHistory() const;
    void clearHistory();
    void setClipboardText(const QString &text);
    void setClipboardFiles(const QStringList &filePaths);
    void removeFromHistory(const QString &text);
    
    // Settings management
    void setMaxHistorySize(int size);
    int getMaxHistorySize() const;
    void setAutoStart(bool enabled);
    bool getAutoStart() const;
    void setShowTrayIcon(bool enabled);
    bool getShowTrayIcon() const;
    
    // Global hotkey management
    void setGlobalHotkey(const QKeySequence &keySequence);
    QKeySequence getGlobalHotkey() const;
    bool isGlobalHotkeyEnabled() const;
    void setGlobalHotkeyEnabled(bool enabled);
    
    void loadSettings();
    void saveSettings();

signals:
    void historyChanged(const QStringList &history);
    void showHistoryRequested();
    void toggleHistoryRequested();

private slots:
    void checkClipboard();
    void onGlobalHotkeyPressed(int id);

private:
    void registerGlobalHotkey();
    void checkClipboardForFiles();
    QClipboard *clipboard;
    QString lastText;
    QStringList lastFiles;
    QStringList history;
    QSettings settings;
    bool selfCopy = false;
    
    // Configurable settings
    int maxHistorySize = 50;
    bool autoStart = false;
    bool showTrayIcon = true;
    QKeySequence globalHotkey = QKeySequence("Ctrl+Shift+V");
    bool globalHotkeyEnabled = true;
    
    // Global hotkey instance
    GlobalHotkey *globalHotkeyManager;
};
