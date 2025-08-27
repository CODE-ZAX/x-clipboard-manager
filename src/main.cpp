#include <QApplication>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QAction>
#include <QTimer>
#include "../include/ClipboardManager.h"
#include "../include/HistoryWindow.h"
#include "../include/PreferencesWindow.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    
    // Set application properties
    app.setApplicationName("Clipboard Manager");
    app.setApplicationVersion("1.0");
    app.setOrganizationName("MyCompany");
    app.setOrganizationDomain("mycompany.com");
    
    // Don't quit when last window is closed
    app.setQuitOnLastWindowClosed(false);

    // Core manager + GUI
    ClipboardManager manager;
    HistoryWindow historyWin(&manager);
    PreferencesWindow prefsWin(&manager);

    // Tray icon
    QSystemTrayIcon tray;
    tray.setIcon(QIcon::fromTheme("edit-paste"));
    tray.setToolTip("Clipboard Manager");
    tray.setVisible(manager.getShowTrayIcon());

    // Tray menu
    QMenu menu;
    QAction showHistoryAction("Show History");
    QAction preferencesAction("Preferences");
    QAction clearAction("Clear History");
    QAction quitAction("Quit");

    QObject::connect(&showHistoryAction, &QAction::triggered, [&]() {
        historyWin.show();
        historyWin.raise();
        historyWin.activateWindow();
    });

    QObject::connect(&preferencesAction, &QAction::triggered, [&]() {
        prefsWin.show();
        prefsWin.raise();
        prefsWin.activateWindow();
    });

    QObject::connect(&clearAction, &QAction::triggered, [&]() {
        manager.clearHistory();
    });

    QObject::connect(&quitAction, &QAction::triggered, [&]() {
        // Clean up before quitting
        manager.saveSettings();
        app.quit();
    });

    menu.addAction(&showHistoryAction);
    menu.addAction(&preferencesAction);
    menu.addSeparator();
    menu.addAction(&clearAction);
    menu.addSeparator();
    menu.addAction(&quitAction);
    tray.setContextMenu(&menu);
    
    // Show tray icon based on settings
    if (manager.getShowTrayIcon()) {
        tray.show();
    }

    // Sync manager → window
    QObject::connect(&manager, &ClipboardManager::historyChanged,
                     &historyWin, &HistoryWindow::updateHistory);
    
    // Connect global hotkey to toggle history
    QObject::connect(&manager, &ClipboardManager::toggleHistoryRequested, [&]() {
        if (historyWin.isVisible()) {
            historyWin.hide();
        } else {
            historyWin.show();
            historyWin.raise();
            historyWin.activateWindow();
        }
    });

    // Initialize window with saved history
    historyWin.updateHistory(manager.getHistory());

    // Handle application state changes
    QObject::connect(&app, &QApplication::aboutToQuit, [&]() {
        manager.saveSettings();
    });

    return app.exec();
}
