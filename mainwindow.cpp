#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

// Public:

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_StatusBar = MainWindow::statusBar();
    m_StatusBar->showMessage("Not Connected");
    grabKeyboard();
}

MainWindow::~MainWindow()
{
    delete ui;
}

// Public slots:

void MainWindow::deviceSelected(QBluetoothDeviceInfo device) {
    static const QString serviceUuid(QStringLiteral("00001101-0000-1000-8000-00805F9B34FB"));

    m_socket = new QBluetoothSocket(QBluetoothServiceInfo::RfcommProtocol, this);
    m_socket->connectToService(QBluetoothAddress(device.address()), \
                           QBluetoothUuid(serviceUuid));
    connect(m_socket, SIGNAL(error(QBluetoothSocket::SocketError)), \
            this, SLOT(socketError(QBluetoothSocket::SocketError)));
    connect(m_socket, SIGNAL(connected()), \
            this, SLOT(onConnected()));
    connect(m_socket, SIGNAL(disconnected()), \
            this, SLOT(disconnected()));
    connect(m_socket, SIGNAL(readyRead()), \
            this, SLOT(readData()));
    connect(m_socket, SIGNAL(stateChanged(QBluetoothSocket::SocketState)), \
            this, SLOT(socketStateChanged(QBluetoothSocket::SocketState)));
    m_pairedDevicesWindow->deleteLater();
}

void MainWindow::socketError(QBluetoothSocket::SocketError error) {
    qDebug() << "Socket error: " << error;
}

void MainWindow::onConnected() {
    qDebug() << "Connected";
}

void MainWindow::disconnected() {
    qDebug() << "Disconnected";
}

void MainWindow::readData() {

}

void MainWindow::socketStateChanged(QBluetoothSocket::SocketState) {

}

// Private Slots:

void MainWindow::laserToggle(bool toggle) {

    if(toggle)
        qDebug() << "LaserToggle toggle: true";
    else
        qDebug() << "LaserToggle toggle: false";
}

void MainWindow::onCannonButtonClick() {
    emit actionCannonOn();
}

void MainWindow::onCannonButtonRelease() {
    emit actionCannonOff();
}

void MainWindow::onMGButtonClick() {
    emit actionMGOn();
}

void MainWindow::onMGButtonRelease() {
    emit actionMGOff();
}

void MainWindow::on_actionListConnectedDevices_triggered()
{
    //PairedDevicesWindow pdw;
    //pdw.setModal(true);
    //pdw.exec();
    m_pairedDevicesWindow = new PairedDevicesWindow(this);
    m_pairedDevicesWindow->show();
}


// Private:

void MainWindow::keyPressEvent(QKeyEvent *k) {
    //qDebug() << k->text() << " Pressed";
    if(!k->isAutoRepeat()) {
        switch(k->key()) {
            case Qt::Key_W:
                qDebug() << "key 'w' pressed";
                emit actionForwardButtonClick();
                break;
            case Qt::Key_A:
                qDebug() << "key 'a' pressed";
                emit actionLeftButtonClick();
                break;
            case Qt::Key_S:
                qDebug() << "key 's' pressed";
                emit actionBackwardButtonClick();
                break;
            case Qt::Key_D:
                qDebug() << "key 'd' pressed";
                emit actionRightButtonClick();
                break;
            case Qt::Key_Q:
                qDebug() << "key 'q' pressed";
                emit actionCannonOn();
                break;
            case Qt::Key_E:
                qDebug() << "key 'e' pressed";
                emit actionLaserToggle();
                break;
            case Qt::Key_Space:
                qDebug() << "key ' ' pressed";
                emit actionMGOn();
                break;
            default:
                break;
        }
    }
}

void MainWindow::keyReleaseEvent(QKeyEvent *k) {
    //qDebug() << k->text() << " Released";
    if(!k->isAutoRepeat()) {
        switch(k->key()) {
            case Qt::Key_W:
                qDebug() << "key 'w' released";
                emit actionForwardButtonRelease();
                break;
            case Qt::Key_A:
                qDebug() << "key 'a' released";
                emit actionLeftButtonRelease();
                break;
            case Qt::Key_S:
                qDebug() << "key 's' released";
                emit actionBackwardButtonRelease();
                break;
            case Qt::Key_D:
                qDebug() << "key 'd' released";
                emit actionRightButtonRelease();
                break;
            case Qt::Key_Q:
                qDebug() << "key 'q' released";
                emit actionCannonOff();
                break;
            case Qt::Key_E:
                qDebug() << "key 'e' released";
                break;
            case Qt::Key_Space:
                qDebug() << "key ' ' released";
                emit actionMGOff();
                break;
            default:
                break;
        }
    }
}