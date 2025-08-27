#include "../include/PreferencesWindow.h"
#include "../include/ClipboardManager.h"
#include <QApplication>
#include <QDialogButtonBox>
#include <QKeySequenceEdit>

PreferencesWindow::PreferencesWindow(ClipboardManager *manager, QWidget *parent)
    : QDialog(parent), clipboardManager(manager) {
    
    setWindowTitle("Preferences");
    setModal(true);
    resize(400, 300);
    
    setupUI();
    loadSettings();
}

void PreferencesWindow::setupUI() {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    
    // History Settings Group
    QGroupBox *historyGroup = new QGroupBox("History Settings", this);
    QVBoxLayout *historyLayout = new QVBoxLayout(historyGroup);
    
    QHBoxLayout *historySizeLayout = new QHBoxLayout();
    QLabel *historySizeLabel = new QLabel("Maximum history size:", this);
    historySizeSpinBox = new QSpinBox(this);
    historySizeSpinBox->setRange(10, 1000);
    historySizeSpinBox->setSuffix(" items");
    historySizeLayout->addWidget(historySizeLabel);
    historySizeLayout->addWidget(historySizeSpinBox);
    historySizeLayout->addStretch();
    
    historyLayout->addLayout(historySizeLayout);
    mainLayout->addWidget(historyGroup);
    
    // Application Settings Group
    QGroupBox *appGroup = new QGroupBox("Application Settings", this);
    QVBoxLayout *appLayout = new QVBoxLayout(appGroup);
    
    autoStartCheckBox = new QCheckBox("Start automatically at login", this);
    showTrayIconCheckBox = new QCheckBox("Show tray icon", this);
    globalHotkeyEnabledCheckBox = new QCheckBox("Enable global hotkey", this);
    globalHotkeyEdit = new QKeySequenceEdit(this);
    globalHotkeyEdit->setToolTip("Press keys for global hotkey...");
    
    appLayout->addWidget(autoStartCheckBox);
    appLayout->addWidget(showTrayIconCheckBox);
    appLayout->addWidget(globalHotkeyEnabledCheckBox);
    
    QHBoxLayout *hotkeyLayout = new QHBoxLayout();
    QLabel *hotkeyLabel = new QLabel("Global hotkey:", this);
    hotkeyLayout->addWidget(hotkeyLabel);
    hotkeyLayout->addWidget(globalHotkeyEdit);
    appLayout->addLayout(hotkeyLayout);
    
    mainLayout->addWidget(appGroup);
    mainLayout->addStretch();
    
    // Buttons
    QDialogButtonBox *buttonBox = new QDialogButtonBox(
        QDialogButtonBox::Save | QDialogButtonBox::Cancel, this);
    
    connect(buttonBox, &QDialogButtonBox::accepted, this, &PreferencesWindow::saveSettings);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
    
    mainLayout->addWidget(buttonBox);
    setLayout(mainLayout);
}

void PreferencesWindow::loadSettings() {
    if (clipboardManager) {
        historySizeSpinBox->setValue(clipboardManager->getMaxHistorySize());
        autoStartCheckBox->setChecked(clipboardManager->getAutoStart());
        showTrayIconCheckBox->setChecked(clipboardManager->getShowTrayIcon());
        globalHotkeyEnabledCheckBox->setChecked(clipboardManager->isGlobalHotkeyEnabled());
        globalHotkeyEdit->setKeySequence(clipboardManager->getGlobalHotkey());
    }
}

void PreferencesWindow::saveSettings() {
    if (clipboardManager) {
        clipboardManager->setMaxHistorySize(historySizeSpinBox->value());
        clipboardManager->setAutoStart(autoStartCheckBox->isChecked());
        clipboardManager->setShowTrayIcon(showTrayIconCheckBox->isChecked());
        clipboardManager->setGlobalHotkeyEnabled(globalHotkeyEnabledCheckBox->isChecked());
        clipboardManager->setGlobalHotkey(globalHotkeyEdit->keySequence());
    }
}
