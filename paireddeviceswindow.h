#ifndef PAIREDDEVICESWINDOW_H
#define PAIREDDEVICESWINDOW_H

#include <QDialog>
#include <QBluetoothDeviceDiscoveryAgent>
#include <QBluetoothDeviceInfo>
#include <QBluetoothSocket>

namespace Ui {
class PairedDevicesWindow;
}

class PairedDevicesWindow : public QDialog
{
    Q_OBJECT

public:
    explicit PairedDevicesWindow(QWidget *parent = 0);
    ~PairedDevicesWindow();
private slots:
    void deviceDiscovered(const QBluetoothDeviceInfo &device);
private:
    Ui::PairedDevicesWindow *ui;
    QList<QBluetoothDeviceInfo> m_dicoveredDevices;

    QBluetoothDeviceDiscoveryAgent* m_discoveryAgent;
};

#endif // PAIREDDEVICESWINDOW_H
