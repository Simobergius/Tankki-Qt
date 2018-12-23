#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>

// Public:

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_worker(new BluetoothWorker(50))
{
    ui->setupUi(this);
    m_StatusBar = MainWindow::statusBar();
    m_StatusBar->showMessage("Not Connected");
    grabKeyboard();

    m_worker->moveToThread(&m_workerThread);

    connect(&m_workerThread, SIGNAL(started()), \
            m_worker, SLOT(onThreadStarted()));
    connect(&m_workerThread, SIGNAL(finished()), \
            m_worker, SLOT(onthreadFinished()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

// Public slots:

void MainWindow::deviceSelected(QBluetoothDeviceInfo device) {
    static const QString serviceUuid(QStringLiteral("00001101-0000-1000-8000-00805F9B34FB"));

    m_socket = new QBluetoothSocket(QBluetoothServiceInfo::RfcommProtocol, this);
    m_socket->connectToService(QBluetoothAddress(device.address()), 1);
    
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
    qDebug() << "Socket error: " << socketErrorToString(error);
    m_StatusBar->showMessage(socketErrorToString(error));
}

void MainWindow::onConnected() {
    qDebug() << "Connected";
    m_StatusBar->showMessage("Connected to " + m_socket->peerName());
    m_worker->setSocket(m_socket);
    m_workerThread.start();
}

void MainWindow::disconnected() {
    qDebug() << "Disconnected";
    m_StatusBar->showMessage("Disconnected");

    if(m_worker) {
        m_worker->stopRequested();
    }

    if(m_workerThread.isRunning()) {
        m_workerThread.quit();
        m_workerThread.wait();
    }
}

void MainWindow::readData() {

}

void MainWindow::socketStateChanged(QBluetoothSocket::SocketState state) {
    qDebug() << "Socket state changed: " << socketStateToString(state);
    m_StatusBar->showMessage("Socket state changed: " + socketStateToString(state));
}

// Private Slots:

void MainWindow::laserToggle(bool toggle) {

    if(toggle)
        qDebug() << "LaserToggle toggle: true";
    else
        qDebug() << "LaserToggle toggle: false";
    m_worker->setLaser(toggle);
}

void MainWindow::onCannonButtonClick() {
    m_worker->setCannon(true);
}

void MainWindow::onCannonButtonRelease() {
    m_worker->setCannon(false);
}

void MainWindow::onMGButtonClick() {
    m_worker->setMG(true);
}

void MainWindow::onMGButtonRelease() {
    m_worker->setMG(false);
}

void MainWindow::on_actionSearchDevices_triggered()
{
    m_pairedDevicesWindow = new PairedDevicesWindow(this);
    m_pairedDevicesWindow->show();
}


// Private:

void MainWindow::keyPressEvent(QKeyEvent *k) {
    //qDebug() << k->text() << " Pressed";
    // TODO: Implement functionality to change worker movement values according to key presses
    if(!k->isAutoRepeat()) {
        switch(k->key()) {
            case Qt::Key_W:
                qDebug() << "key 'w' pressed";
                break;
            case Qt::Key_A:
                qDebug() << "key 'a' pressed";
                break;
            case Qt::Key_S:
                qDebug() << "key 's' pressed";
                break;
            case Qt::Key_D:
                qDebug() << "key 'd' pressed";
                break;
            case Qt::Key_Q:
                qDebug() << "key 'q' pressed";
                m_worker->setCannon(true);
                break;
            case Qt::Key_E:
                qDebug() << "key 'e' pressed";
                emit actionLaserToggle();
                break;
            case Qt::Key_Space:
                qDebug() << "key ' ' pressed";
                m_worker->setMG(true);
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
                break;
            case Qt::Key_A:
                qDebug() << "key 'a' released";
                break;
            case Qt::Key_S:
                qDebug() << "key 's' released";
                break;
            case Qt::Key_D:
                qDebug() << "key 'd' released";
                break;
            case Qt::Key_Q:
                qDebug() << "key 'q' released";
                m_worker->setCannon(false);
                break;
            case Qt::Key_E:
                qDebug() << "key 'e' released";
                break;
            case Qt::Key_Space:
                qDebug() << "key ' ' released";
                m_worker->setMG(false);
                break;
            default:
                break;
        }
    }
}

QString MainWindow::socketErrorToString(QBluetoothSocket::SocketError err) {
    switch(err) {
        case QBluetoothSocket::UnknownSocketError:
            return QString("An unknown error has occurred.");
            break;
        case QBluetoothSocket::NoSocketError:
            return QString("No error. Used for testing.");
            break;
        case QBluetoothSocket::HostNotFoundError:
            return QString("Could not find the remote host.");
            break;
        case QBluetoothSocket::ServiceNotFoundError:
            return QString("Could not find the service UUID on remote host.");
            break;
        case QBluetoothSocket::NetworkError:
            return QString("Attempt to read or write from socket returned an error");
            break;
        case QBluetoothSocket::UnsupportedProtocolError:
            return QString("The Protocol is not supported on this platform.");
            break;
        case QBluetoothSocket::OperationError:
            return QString("An operation was attempted while the socket was in a state that did not permit it.");
            break;
        case QBluetoothSocket::RemoteHostClosedError:
            return QString("The remote host closed the connection. This value was introduced by Qt 5.10.");
            break;
        default:
            return QString();
    }
}

QString MainWindow::socketStateToString(QBluetoothSocket::SocketState s) {
    switch(s) {
        case QBluetoothSocket::UnconnectedState:
            return QString("Socket is not connected.");
            break;
        case QBluetoothSocket::ServiceLookupState:
            return QString("Socket is querying connection parameters.");
            break;
        case QBluetoothSocket::ConnectingState:
            return QString("Socket is attempting to connect to a device.");
            break;
        case QBluetoothSocket::ConnectedState:
            return QString("Socket is connected to a device.");
            break;
        case QBluetoothSocket::BoundState:
            return QString("Socket is bound to a local address and port.");
            break;
        case QBluetoothSocket::ClosingState:
            return QString("Socket is connected and will be closed once all pending data is written to the socket.");
            break;
        case QBluetoothSocket::ListeningState:
            return QString("Socket is listening for incoming connections.");
            break;
        default:
            return QString();
            break;
    }
}