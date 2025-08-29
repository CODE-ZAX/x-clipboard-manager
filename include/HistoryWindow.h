#pragma once
#include <QWidget>
#include <QListWidget>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QMenu>
#include <QFileInfo>
#include <QStyledItemDelegate>
#include <QTextDocument>
#include <QTimer>
#include <QLabel>
#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>


class ClipboardManager; // forward declaration



// Custom delegate for list items with hover delete button
class HistoryItemDelegate : public QStyledItemDelegate {
    Q_OBJECT

public:
    explicit HistoryItemDelegate(QObject *parent = nullptr);
    
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index) override;

signals:
    void deleteItemRequested(int row);

private:
    mutable QRect deleteButtonRect;
    mutable bool isHovering;
};

class HistoryWindow : public QWidget {
    Q_OBJECT

public:
    explicit HistoryWindow(ClipboardManager *manager, QWidget *parent = nullptr);

public slots:
    void updateHistory(const QStringList &history);
    void filterHistory(const QString &filter);
    void showWindow(); // Show window without hiding on copy

protected:
    void closeEvent(QCloseEvent *event) override;
    void contextMenuEvent(QContextMenuEvent *event) override;

private slots:
    void onItemDoubleClicked(QListWidgetItem *item);
    void copySelectedItem();
    void copySelectedItemAsFiles();
    void removeSelectedItem();
    void clearAllItems();
    void onDeleteItemRequested(int row);
    void onItemClicked(QListWidgetItem *item);

private:
    void setupUI();
    void setupContextMenu();
    bool isFileEntry(const QString &text) const;
    QString formatTextForDisplay(const QString &text) const;
    QString truncateText(const QString &text, int maxLines = 4) const;
    void createListItem(const QString &text, int index);
    void showCopyNotification();
    
    QStringList originalHistory;
    QStringList filteredHistory;
    
    QListWidget *listWidget;
    QLineEdit *searchBox;
    ClipboardManager *clipboardManager;
    QMenu *contextMenu;
    HistoryItemDelegate *itemDelegate;
    QTimer *hideTimer;
    bool shouldHideAfterCopy;
    QLabel *notificationLabel;
    QGraphicsOpacityEffect *notificationOpacity;
    QPropertyAnimation *notificationAnimation;
};
