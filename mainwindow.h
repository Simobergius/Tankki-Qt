#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "paireddeviceswindow.h"
#include "settingsdialog.h"
#include "bluetoothworker.h"
#include "touchareawidget.h"

#include <QMainWindow>
#include <QPushButton>
#include <QKeyEvent>
#include <QThread>
#include <QTimerEvent>
#include <QSettings>

namespace Ui {
class MainWindow;
}

enum InputType {
    MouseInput,
    KeyboardInput
};

struct Settings {
    InputType inputType;
    int maxPower;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
signals:
    void actionLaserToggle();
    void setRightForwardPower(int);
    void setRightBackwardPower(int);
    void setLeftForwardPower(int);
    void setLeftBackwardPower(int);
    void actionListConnectedDevices();

public slots:
    void deviceSelected(QBluetoothDeviceInfo);

    void socketError(QBluetoothSocket::SocketError);
    void onConnected();
    void disconnected();
    void readData();
    void socketStateChanged(QBluetoothSocket::SocketState);

private slots:
    void laserToggle(bool);
    void onCannonButtonClick();
    void onCannonButtonRelease();
    void onMGButtonClick();
    void onMGButtonRelease();

    void on_actionSearchDevices_triggered();
    void on_actionDisconnect_triggered();
    void on_actionSettings_triggered();

protected:
    virtual void timerEvent(QTimerEvent *e);
private:
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);

    QString socketErrorToString(QBluetoothSocket::SocketError);
    QString socketStateToString(QBluetoothSocket::SocketState);

    static InputType strToInputType(QString str);

    Ui::MainWindow *ui;
    QStatusBar* m_StatusBar;
    TouchAreaWidget* m_touchAreaWidget;
    PairedDevicesWindow *m_pairedDevicesWindow;
    SettingsDialog *m_settingsDialog;
    QBluetoothSocket* m_socket;
    BluetoothWorker* m_worker;
    QThread m_workerThread;

    int m_timerId;

    QSettings *m_settings;

    bool m_forwardPressed, m_backwardPressed, m_leftPressed, m_rightPressed;
    int m_speedForwardBackward, m_speedLeftRight;
};

#endif // MAINWINDOW_H
