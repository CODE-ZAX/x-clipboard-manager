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
#include <QPainter>
#include <QStyle>
#include <QTextDocument>
#include <QTextOption>
#include <QMouseEvent>
#include <QPushButton>
#include <QStyleOptionButton>
#include <QTimer>
#include <QLabel>
#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>
#include <QScreen>



// HistoryItemDelegate implementation
HistoryItemDelegate::HistoryItemDelegate(QObject *parent)
    : QStyledItemDelegate(parent), isHovering(false) {
}

void HistoryItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const {
    QStyleOptionViewItem opt = option;
    initStyleOption(&opt, index);
    
    // Draw the background with proper hover and selection states
    if (opt.state & QStyle::State_Selected) {
        // Selected item - blue highlight
        painter->fillRect(opt.rect, opt.palette.highlight());
    } else if (opt.state & QStyle::State_MouseOver) {
        // Hover effect - light blue/gray highlight
        QColor hoverColor = opt.palette.highlight().color().lighter(150);
        painter->fillRect(opt.rect, hoverColor);
    } else {
        // Normal background with alternating row colors
        if (index.row() % 2 == 0) {
            painter->fillRect(opt.rect, opt.palette.alternateBase().color().lighter(110));
        } else {
            painter->fillRect(opt.rect, opt.palette.base());
        }
    }
    
    // Calculate text rectangle (leave space for delete button)
    QRect textRect = opt.rect;
    textRect.setRight(textRect.right() - 30); // Space for delete button
    
    // Get the text content
    QString text = index.data(Qt::DisplayRole).toString();
    
    // Create text document for rich text rendering
    QTextDocument doc;
    doc.setPlainText(text);
    doc.setDefaultTextOption(QTextOption(Qt::AlignLeft | Qt::AlignTop));
    
    // Set document width
    doc.setTextWidth(textRect.width() - 10);
    
    // Draw the text
    painter->save();
    painter->translate(textRect.left() + 5, textRect.top() + 5);
    doc.drawContents(painter);
    painter->restore();
    
    // Always draw delete button (simple gray X)
    QRect deleteRect = opt.rect;
    deleteRect.setLeft(deleteRect.right() - 25);
    deleteRect.setTop(deleteRect.top() + (deleteRect.height() - 16) / 2);
    deleteRect.setHeight(16);
    deleteRect.setWidth(16);
    
    deleteButtonRect = deleteRect;
    
    // Draw simple X symbol
    painter->save();
    painter->setPen(QPen(Qt::gray, 1.5));
    
    int margin = 4;
    QRect xRect = deleteRect.adjusted(margin, margin, -margin, -margin);
    painter->drawLine(xRect.topLeft(), xRect.bottomRight());
    painter->drawLine(xRect.topRight(), xRect.bottomLeft());
    
    painter->restore();
}

QSize HistoryItemDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const {
    QString text = index.data(Qt::DisplayRole).toString();
    
    // Calculate height based on text content
    QTextDocument doc;
    doc.setPlainText(text);
    doc.setDefaultTextOption(QTextOption(Qt::AlignLeft | Qt::AlignTop));
    doc.setTextWidth(option.rect.width() - 40); // Account for margins and delete button
    
    int height = qMax(30, (int)doc.size().height() + 20); // Minimum height of 30
    return QSize(option.rect.width(), height);
}

bool HistoryItemDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index) {
    if (event->type() == QEvent::MouseButtonRelease) {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
        QRect deleteRect = option.rect;
        deleteRect.setLeft(deleteRect.right() - 25);
        deleteRect.setTop(deleteRect.top() + (deleteRect.height() - 16) / 2);
        deleteRect.setHeight(16);
        deleteRect.setWidth(16);
        
        if (deleteRect.contains(mouseEvent->pos())) {
            emit deleteItemRequested(index.row());
            return true;
        }
    }
    
    return QStyledItemDelegate::editorEvent(event, model, option, index);
}

// HistoryWindow implementation
HistoryWindow::HistoryWindow(ClipboardManager *manager, QWidget *parent)
    : QWidget(parent), clipboardManager(manager), shouldHideAfterCopy(true) {

    setWindowTitle("Xclipy - Clipboard History");
    resize(500, 400);

    // Always stay on top of all applications
    setWindowFlags(Qt::Window | Qt::WindowStaysOnTopHint | Qt::Tool);
    
    // Set window attributes for better top-level behavior
    setAttribute(Qt::WA_ShowWithoutActivating, false);
    setAttribute(Qt::WA_TranslucentBackground, false);

    setupUI();
    setupContextMenu();

    // Copy from history using ClipboardManager
    connect(listWidget, &QListWidget::itemClicked, this, &HistoryWindow::onItemClicked);

    // Double-click to copy
    connect(listWidget, &QListWidget::itemDoubleClicked, this, &HistoryWindow::onItemDoubleClicked);
    
    // Setup hide timer
    hideTimer = new QTimer(this);
    hideTimer->setSingleShot(true);
    connect(hideTimer, &QTimer::timeout, this, &QWidget::hide);
    
    // Setup notification label
    notificationLabel = new QLabel("Item copied!", this);
    notificationLabel->setStyleSheet(
        "QLabel { "
        "background-color: rgba(0, 0, 0, 180); "
        "color: white; "
        "border-radius: 15px; "
        "padding: 10px 20px; "
        "font-size: 14px; "
        "font-weight: bold; "
        "}"
    );
    notificationLabel->setAlignment(Qt::AlignCenter);
    notificationLabel->setFixedSize(150, 40);
    notificationLabel->hide();
    
    // Setup opacity effect and animation
    notificationOpacity = new QGraphicsOpacityEffect(this);
    notificationLabel->setGraphicsEffect(notificationOpacity);
    
    notificationAnimation = new QPropertyAnimation(notificationOpacity, "opacity", this);
    notificationAnimation->setDuration(2000);
    notificationAnimation->setStartValue(1.0);
    notificationAnimation->setEndValue(0.0);
    
    connect(notificationAnimation, &QPropertyAnimation::finished, [this]() {
        notificationLabel->hide();
    });
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
    

    
    // Enable mouse tracking for hover effects
    listWidget->setMouseTracking(true);
    
    // Enable resizable columns (for future table view)
    listWidget->setUniformItemSizes(false);
    
    // Set custom delegate
    itemDelegate = new HistoryItemDelegate(this);
    listWidget->setItemDelegate(itemDelegate);
    
    // Connect delete signal
    connect(itemDelegate, &HistoryItemDelegate::deleteItemRequested, this, &HistoryWindow::onDeleteItemRequested);
    
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
        for (int i = 0; i < history.size(); ++i) {
            createListItem(history[i], i);
        }
    }
}

void HistoryWindow::createListItem(const QString &text, int index) {
    Q_UNUSED(index)
    QListWidgetItem *item = new QListWidgetItem();
    
    // Format text for display with truncation
    QString displayText = formatTextForDisplay(text);
    item->setText(displayText);
    item->setData(Qt::UserRole, text); // Store original text
    
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

QString HistoryWindow::formatTextForDisplay(const QString &text) const {
    // Check if text contains multiple lines (list-like content)
    QStringList lines = text.split("\n", Qt::SkipEmptyParts);
    
    if (lines.size() > 1) {
        // Add bullet points for list items
        QStringList formattedLines;
        for (const QString &line : lines) {
            if (!line.trimmed().isEmpty()) {
                formattedLines.append("• " + line.trimmed());
            }
        }
        return truncateText(formattedLines.join("\n"));
    } else {
        // Single line text
        return truncateText(text);
    }
}

QString HistoryWindow::truncateText(const QString &text, int maxLines) const {
    QStringList lines = text.split("\n", Qt::SkipEmptyParts);
    
    if (lines.size() <= maxLines) {
        return text;
    }
    
    // Truncate to maxLines and add ellipsis
    QStringList truncatedLines = lines.mid(0, maxLines);
    truncatedLines.append("...");
    return truncatedLines.join("\n");
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
    
    for (int i = 0; i < filteredHistory.size(); ++i) {
        createListItem(filteredHistory[i], i);
    }
}

void HistoryWindow::onItemClicked(QListWidgetItem *item) {
    if (item && clipboardManager) {
        QString originalText = item->data(Qt::UserRole).toString();
        if (isFileEntry(originalText)) {
            clipboardManager->setClipboardFiles(originalText.split("\n", Qt::SkipEmptyParts));
        } else {
            clipboardManager->setClipboardText(originalText);
        }
        
        // Show copy notification
        showCopyNotification();
        
        // Hide window after a short delay if configured
        if (shouldHideAfterCopy) {
            hideTimer->start(500); // Hide after 500ms
        }
    }
}

void HistoryWindow::onItemDoubleClicked(QListWidgetItem *item) {
    Q_UNUSED(item)
    onItemClicked(listWidget->currentItem());
}

void HistoryWindow::copySelectedItem() {
    QListWidgetItem *currentItem = listWidget->currentItem();
    if (currentItem && clipboardManager) {
        QString originalText = currentItem->data(Qt::UserRole).toString();
        clipboardManager->setClipboardText(originalText);
    }
}

void HistoryWindow::copySelectedItemAsFiles() {
    QListWidgetItem *currentItem = listWidget->currentItem();
    if (currentItem && clipboardManager) {
        QString originalText = currentItem->data(Qt::UserRole).toString();
        if (isFileEntry(originalText)) {
            QStringList files = originalText.split("\n", Qt::SkipEmptyParts);
            clipboardManager->setClipboardFiles(files);
        } else {
            // If it's not a file entry, copy as text
            clipboardManager->setClipboardText(originalText);
        }
    }
}

void HistoryWindow::onDeleteItemRequested(int row) {
    QListWidgetItem *item = listWidget->item(row);
    if (item) {
        QString textToRemove = item->data(Qt::UserRole).toString();
        
        // Remove from manager's history (this will update our display via signal)
        if (clipboardManager) {
            clipboardManager->removeFromHistory(textToRemove);
        }
    }
}

void HistoryWindow::removeSelectedItem() {
    QListWidgetItem *currentItem = listWidget->currentItem();
    if (currentItem) {
        QString textToRemove = currentItem->data(Qt::UserRole).toString();
        
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

void HistoryWindow::showWindow() {
    shouldHideAfterCopy = false;
    show();
    raise();
    activateWindow();
    // Reset the flag after a short delay
    QTimer::singleShot(1000, [this]() { shouldHideAfterCopy = true; });
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



void HistoryWindow::showCopyNotification() {
    // Position notification at center of window
    int x = (width() - notificationLabel->width()) / 2;
    int y = (height() - notificationLabel->height()) / 2;
    notificationLabel->move(x, y);
    
    // Show and animate
    notificationLabel->show();
    notificationLabel->raise();
    notificationOpacity->setOpacity(1.0);
    notificationAnimation->start();
}

// Hide instead of quit on close
void HistoryWindow::closeEvent(QCloseEvent *event) {
    event->ignore();   // don't close the app
    this->hide();      // just hide the window
}
