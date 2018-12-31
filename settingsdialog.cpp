#include "settingsdialog.h"
#include "ui_settingsdialog.h"

#include <QSlider>
#include <QDebug>

SettingsDialog::SettingsDialog(QWidget *parent, QSettings *settings) :
    QDialog(parent), \
    ui(new Ui::SettingsDialog), m_settings(settings), \
    m_maxPowerSlider(findChild<QSlider*>("maximumPowerSlider"))
{
    ui->setupUi(this);
    m_controlMethodButtonGroup.addButton(findChild<QRadioButton*>("radioButtonKeyboardControl"), 0);
    m_controlMethodButtonGroup.addButton(findChild<QRadioButton*>("radioButtonMouseControl"), 1);
    m_maxPowerSlider = findChild<QSlider*>("maximumPowerSlider");

    // Set Radiobutton and slider values to current settings
    QString controlType = m_settings->value("trackControlMethod").toString();
    qDebug() << controlType;

    if (controlType == "keyboardInput") {
        m_controlMethodButtonGroup.button(0)->setChecked(true);
    }
    else if (controlType == "mouseInput") {
        m_controlMethodButtonGroup.button(0)->setChecked(true);
    }

    int maxPower = m_settings->value("maximumPower").toInt();
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}

void SettingsDialog::accept() {
    switch (m_controlMethodButtonGroup.checkedId()) {
    case 0:
        m_settings->setValue("trackControlMethod", QVariant("keyboardInput"));
        break;
    case 1:
        m_settings->setValue("trackcontrolMethod", QVariant("mouseInput"));
        break;
    default:
        break;
    }

    m_settings->setValue("maximumPower", QVariant(m_maxPowerSlider->value()));

    done(QDialog::Accepted);
}

void SettingsDialog::reject() {
    done(QDialog::Rejected);

}