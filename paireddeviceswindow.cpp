#include "paireddeviceswindow.h"
#include "ui_paireddeviceswindow.h"
#include "devicebutton.h"

#include <QDebug>
#include <QVBoxLayout>

PairedDevicesWindow::PairedDevicesWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PairedDevicesWindow),
    m_discoveryAgent(new QBluetoothDeviceDiscoveryAgent(this))
{
    ui->setupUi(this);
    connect(m_discoveryAgent, SIGNAL(deviceDiscovered(QBluetoothDeviceInfo)), \
            this, SLOT(deviceDiscovered(QBluetoothDeviceInfo)));

    m_discoveryAgent->start();
}

PairedDevicesWindow::~PairedDevicesWindow()
{
    m_discoveryAgent->stop();
    delete ui;
}

void PairedDevicesWindow::deviceDiscovered(const QBluetoothDeviceInfo &device) {
    qDebug() << "found device: " << device.name() << ", address: " << device.address().toString();
    m_dicoveredDevices.append(device);
    DeviceButton* button = new DeviceButton(device, this);
    button->setText(device.name() + "\n" + device.address().toString());
    connect(button, SIGNAL(deviceSelected(QBluetoothDeviceInfo)), parent(), SLOT(deviceSelected(QBluetoothDeviceInfo)));
    QVBoxLayout *layout = findChild<QVBoxLayout *>("verticalLayout");
    if(layout) {
        layout->addWidget(button);
    }
}
