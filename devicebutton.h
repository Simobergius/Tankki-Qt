#ifndef DEVICEBUTTON_H
#define DEVICEBUTTON_H

#include <QBluetoothDeviceInfo>
#include <QBluetoothUuid>
#include <QBluetoothAddress>
#include <QPushButton>
#include <QDebug>

class DeviceButton : public QPushButton {
    Q_OBJECT
public:
    explicit DeviceButton(const QBluetoothDeviceInfo &device, QWidget *parent = 0);
    ~DeviceButton();

signals:
    void deviceSelected(QBluetoothDeviceInfo);
private slots:
    void buttonClicked(bool);
private:
    QBluetoothDeviceInfo m_device;
};

#endif // DEVICEBUTTON_H