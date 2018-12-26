#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include <QSettings>
#include <QButtonGroup>
#include <QSlider>

namespace Ui {
class SettingsDialog;
}

class SettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsDialog(QWidget *parent = nullptr, QSettings *settings = 0);
    ~SettingsDialog();
private slots:
    void accept();
    void reject();
private:
    Ui::SettingsDialog *ui;
    QSettings *m_settings;

    QButtonGroup m_controlMethodButtonGroup;
    QSlider *m_maxPowerSlider;
};

#endif // SETTINGSDIALOG_H
