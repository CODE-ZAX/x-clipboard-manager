#pragma once
#include <QWidget>
#include <QListWidget>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QMenu>
#include <QFileInfo>

class ClipboardManager; // forward declaration

class HistoryWindow : public QWidget {
    Q_OBJECT

public:
    explicit HistoryWindow(ClipboardManager *manager, QWidget *parent = nullptr);

public slots:
    void updateHistory(const QStringList &history);
    void filterHistory(const QString &filter);

protected:
    void closeEvent(QCloseEvent *event) override;
    void contextMenuEvent(QContextMenuEvent *event) override;

private slots:
    void onItemDoubleClicked(QListWidgetItem *item);
    void copySelectedItem();
    void copySelectedItemAsFiles();
    void removeSelectedItem();
    void clearAllItems();

private:
    void setupUI();
    void setupContextMenu();
    bool isFileEntry(const QString &text) const;
    QStringList originalHistory;
    QStringList filteredHistory;
    
    QListWidget *listWidget;
    QLineEdit *searchBox;
    ClipboardManager *clipboardManager;
    QMenu *contextMenu;
};
