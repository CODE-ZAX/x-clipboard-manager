#pragma once
#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSpinBox>
#include <QCheckBox>
#include <QPushButton>
#include <QLabel>
#include <QGroupBox>
#include <QKeySequenceEdit>

class ClipboardManager;

class PreferencesWindow : public QDialog {
    Q_OBJECT

public:
    explicit PreferencesWindow(ClipboardManager *manager, QWidget *parent = nullptr);

private slots:
    void saveSettings();
    void loadSettings();

private:
    void setupUI();
    
    ClipboardManager *clipboardManager;
    
    QSpinBox *historySizeSpinBox;
    QCheckBox *autoStartCheckBox;
    QCheckBox *showTrayIconCheckBox;
    QCheckBox *globalHotkeyEnabledCheckBox;
    QKeySequenceEdit *globalHotkeyEdit;
    QPushButton *saveButton;
    QPushButton *cancelButton;
};
