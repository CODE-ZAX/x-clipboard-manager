#include "../include/HistoryWindow.h"
#include "../include/ClipboardManager.h"
#include <QApplication>
#include <QCloseEvent>
#include <QListWidgetItem>
#include <QContextMenuEvent>
#include <QHBoxLayout>
#include <QLabel>
#include <QAction>
#include <QHeaderView>
#include <QLineEdit>
#include <QMenu>
#include <QAbstractItemView>

HistoryWindow::HistoryWindow(ClipboardManager *manager, QWidget *parent)
    : QWidget(parent), clipboardManager(manager) {

    setWindowTitle("Clipboard History");
    resize(500, 400);

    // Always stay on top
    setWindowFlags(Qt::Window | Qt::WindowStaysOnTopHint);

    setupUI();
    setupContextMenu();

    // Copy from history using ClipboardManager
    connect(listWidget, &QListWidget::itemClicked, this, [=](QListWidgetItem *item) {
        if (clipboardManager) {
            clipboardManager->setClipboardText(item->text());
        } else {
            QApplication::clipboard()->setText(item->text()); // fallback
        }
    });

    // Double-click to copy
    connect(listWidget, &QListWidget::itemDoubleClicked, this, &HistoryWindow::onItemDoubleClicked);
}

void HistoryWindow::setupUI() {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    
    // Search box
    QHBoxLayout *searchLayout = new QHBoxLayout();
    QLabel *searchLabel = new QLabel("Search:", this);
    searchBox = new QLineEdit(this);
    searchBox->setPlaceholderText("Type to filter history...");
    searchBox->setClearButtonEnabled(true);
    
    searchLayout->addWidget(searchLabel);
    searchLayout->addWidget(searchBox);
    mainLayout->addLayout(searchLayout);
    
    // List widget
    listWidget = new QListWidget(this);
    listWidget->setWordWrap(true);
    listWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    listWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    listWidget->setAlternatingRowColors(true);
    listWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    
    // Enable resizable columns (for future table view)
    listWidget->setUniformItemSizes(false);
    
    mainLayout->addWidget(listWidget);
    setLayout(mainLayout);
    
    // Connect search box
    connect(searchBox, &QLineEdit::textChanged, this, &HistoryWindow::filterHistory);
}

void HistoryWindow::setupContextMenu() {
    contextMenu = new QMenu(this);
    
    QAction *copyAction = new QAction("Copy as Text", this);
    QAction *copyAsFilesAction = new QAction("Copy as Files", this);
    QAction *removeAction = new QAction("Remove", this);
    QAction *clearAllAction = new QAction("Clear All", this);
    
    connect(copyAction, &QAction::triggered, this, &HistoryWindow::copySelectedItem);
    connect(copyAsFilesAction, &QAction::triggered, this, &HistoryWindow::copySelectedItemAsFiles);
    connect(removeAction, &QAction::triggered, this, &HistoryWindow::removeSelectedItem);
    connect(clearAllAction, &QAction::triggered, this, &HistoryWindow::clearAllItems);
    
    contextMenu->addAction(copyAction);
    contextMenu->addAction(copyAsFilesAction);
    contextMenu->addSeparator();
    contextMenu->addAction(removeAction);
    contextMenu->addAction(clearAllAction);
}

void HistoryWindow::updateHistory(const QStringList &history) {
    originalHistory = history;
    filteredHistory = history;
    
    // Apply current filter
    if (!searchBox->text().isEmpty()) {
        filterHistory(searchBox->text());
    } else {
        listWidget->clear();
        for (const QString &text : history) {
            QListWidgetItem *item = new QListWidgetItem(text);
            item->setToolTip(text); // Show full text on hover
            
            // Set icon for file/folder entries
            if (isFileEntry(text)) {
                QStringList files = text.split("\n", Qt::SkipEmptyParts);
                if (!files.isEmpty()) {
                    QFileInfo fileInfo(files.first());
                    if (fileInfo.isDir()) {
                        item->setIcon(QIcon::fromTheme("folder"));
                    } else {
                        item->setIcon(QIcon::fromTheme("text-x-generic"));
                    }
                }
            } else {
                item->setIcon(QIcon::fromTheme("text-x-generic"));
            }
            
            listWidget->addItem(item);
        }
    }
}

void HistoryWindow::filterHistory(const QString &filter) {
    listWidget->clear();
    
    if (filter.isEmpty()) {
        filteredHistory = originalHistory;
    } else {
        filteredHistory.clear();
        for (const QString &text : originalHistory) {
            if (text.contains(filter, Qt::CaseInsensitive)) {
                filteredHistory.append(text);
            }
        }
    }
    
    for (const QString &text : filteredHistory) {
        QListWidgetItem *item = new QListWidgetItem(text);
        item->setToolTip(text);
        
        // Set icon for file/folder entries
        if (isFileEntry(text)) {
            QStringList files = text.split("\n", Qt::SkipEmptyParts);
            if (!files.isEmpty()) {
                QFileInfo fileInfo(files.first());
                if (fileInfo.isDir()) {
                    item->setIcon(QIcon::fromTheme("folder"));
                } else {
                    item->setIcon(QIcon::fromTheme("text-x-generic"));
                }
            }
        } else {
            item->setIcon(QIcon::fromTheme("text-x-generic"));
        }
        
        listWidget->addItem(item);
    }
}

void HistoryWindow::onItemDoubleClicked(QListWidgetItem *item) {
    Q_UNUSED(item)
    QListWidgetItem *currentItem = listWidget->currentItem();
    if (currentItem) {
        QString text = currentItem->text();
        if (isFileEntry(text)) {
            copySelectedItemAsFiles();
        } else {
            copySelectedItem();
        }
    }
}

void HistoryWindow::copySelectedItem() {
    QListWidgetItem *currentItem = listWidget->currentItem();
    if (currentItem && clipboardManager) {
        clipboardManager->setClipboardText(currentItem->text());
    }
}

void HistoryWindow::copySelectedItemAsFiles() {
    QListWidgetItem *currentItem = listWidget->currentItem();
    if (currentItem && clipboardManager) {
        QString text = currentItem->text();
        if (isFileEntry(text)) {
            QStringList files = text.split("\n", Qt::SkipEmptyParts);
            clipboardManager->setClipboardFiles(files);
        } else {
            // If it's not a file entry, copy as text
            clipboardManager->setClipboardText(text);
        }
    }
}

void HistoryWindow::removeSelectedItem() {
    QListWidgetItem *currentItem = listWidget->currentItem();
    if (currentItem) {
        QString textToRemove = currentItem->text();
        
        // Remove from manager's history (this will update our display via signal)
        if (clipboardManager) {
            clipboardManager->removeFromHistory(textToRemove);
        }
    }
}

void HistoryWindow::clearAllItems() {
    if (clipboardManager) {
        clipboardManager->clearHistory();
    }
}

bool HistoryWindow::isFileEntry(const QString &text) const {
    // Check if the text contains file paths
    QStringList lines = text.split("\n", Qt::SkipEmptyParts);
    if (lines.isEmpty()) return false;
    
    // Check if all lines are valid file paths
    for (const QString &line : lines) {
        QFileInfo fileInfo(line.trimmed());
        if (!fileInfo.exists()) {
            return false;
        }
    }
    
    return true;
}

void HistoryWindow::contextMenuEvent(QContextMenuEvent *event) {
    QListWidgetItem *item = listWidget->itemAt(listWidget->mapFromParent(event->pos()));
    if (item) {
        listWidget->setCurrentItem(item);
        contextMenu->exec(event->globalPos());
    }
}

// Hide instead of quit on close
void HistoryWindow::closeEvent(QCloseEvent *event) {
    event->ignore();   // don't close the app
    this->hide();      // just hide the window
}
