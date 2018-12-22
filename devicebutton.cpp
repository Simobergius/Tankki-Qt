#include "devicebutton.h"

DeviceButton::DeviceButton(const QBluetoothDeviceInfo &device, QWidget *parent) :
    m_device(device), QPushButton(parent) 
{
    connect(this, SIGNAL(clicked(bool)), this, SLOT(buttonClicked(bool)));
}

DeviceButton::~DeviceButton() {

}

void DeviceButton::buttonClicked(bool) {
    qDebug() << "Selected device: " << m_device.name() << ", address: " << m_device.address().toString();
    QList<QBluetoothUuid> uuids = m_device.serviceUuids();

    foreach(const QBluetoothUuid& uuid, uuids) {
        qDebug() << uuid;
    }

    emit deviceSelected(m_device);
}