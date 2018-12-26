#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "math_utils.h"

#include <QDebug>

// Public:

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::MainWindow), \
    m_socket(0), m_worker(new BluetoothWorker(50)), m_timerId(-1), \
    m_settings(new QSettings("user_settings.ini", QSettings::NativeFormat, this)), \
    m_forwardPressed(false), m_backwardPressed(false), \
    m_leftPressed(false), m_rightPressed(false), \
    m_speedForwardBackward(0), m_speedLeftRight(0)
{
    ui->setupUi(this);
    m_StatusBar = MainWindow::statusBar();
    m_StatusBar->showMessage("Not Connected");
    //m_touchAreaWidget = findChild<TouchAreaWidget*>("movementControlWidget");
    grabKeyboard();

    m_worker->moveToThread(&m_workerThread);

    m_timerId = startTimer(50);

    connect(&m_workerThread, SIGNAL(started()), \
            m_worker, SLOT(onThreadStarted()));
    connect(&m_workerThread, SIGNAL(finished()), \
            m_worker, SLOT(onThreadFinished()));
}

MainWindow::~MainWindow()
{
    delete ui;

    if(m_workerThread.isRunning()) {
        m_workerThread.quit();
        m_workerThread.wait();
    }
}

// Public slots:

void MainWindow::deviceSelected(QBluetoothDeviceInfo device) {

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

void MainWindow::on_actionDisconnect_triggered() {
    if(m_socket) {
        m_socket->disconnectFromService();
    }
}

void MainWindow::on_actionSettings_triggered() {
    m_settingsDialog = new SettingsDialog(this, m_settings);
    m_settingsDialog->show();
}

// Protected:

void MainWindow::timerEvent(QTimerEvent *e) {
    if(e->timerId() == m_timerId) {
        if(m_settings) {
            switch (strToInputType(m_settings->value("trackControlMethod", QVariant("keyboardControl")).toString())) {
            case KeyboardInput:
                if(m_forwardPressed && !m_backwardPressed) {
                    if (m_speedForwardBackward + 5 < 127) {
                        m_speedForwardBackward += 5;
                    }
                    else {
                        m_speedForwardBackward = 127;
                    }
                }else if(!m_forwardPressed && m_backwardPressed) {
                    if(m_speedForwardBackward - 5 > -127) {
                        m_speedForwardBackward -= 5;
                    }
                    else {
                        m_speedForwardBackward = -127;
                    }
                }
                else {
                    m_speedForwardBackward = 0;
                }

                if(!m_leftPressed && m_rightPressed) {
                    if(m_speedLeftRight + 5 < 127) {
                        m_speedLeftRight += 5;
                    }
                    else {
                        m_speedLeftRight = 127;
                    }
                }
                else if(m_leftPressed && !m_rightPressed) {
                    if(m_speedLeftRight - 5 > -127) {
                        m_speedLeftRight -= 5;
                    }
                    else {
                        m_speedLeftRight = -127;
                    }
                }
                else {
                    m_speedLeftRight = 0;
                }
                break;
        
            case MouseInput:
                qDebug() << "Mouse Input selected";
                break;
            default:
                break;
            }

            m_worker->setSpeedForwardBackward(m_speedForwardBackward);
            m_worker->setSpeedLeftRight(m_speedLeftRight);

            emit setRightForwardPower(MathUtils::map(m_worker->getRightTrackSpeed(), 127, 254, 0, 100));
            emit setRightBackwardPower(MathUtils::map(m_worker->getRightTrackSpeed(), 127, 0, 0, 100));
            emit setLeftForwardPower(MathUtils::map(m_worker->getLeftTrackSpeed(), 127, 254, 0, 100));
            emit setLeftBackwardPower(MathUtils::map(m_worker->getLeftTrackSpeed(), 127, 0, 0, 100));
        }
    }
}

// Private:

void MainWindow::keyPressEvent(QKeyEvent *k) {
    if(!k->isAutoRepeat()) {
        switch(k->key()) {
            case Qt::Key_W:
                qDebug() << "key 'w' pressed";
                m_forwardPressed = true;
                break;
            case Qt::Key_A:
                qDebug() << "key 'a' pressed";
                m_leftPressed = true;
                break;
            case Qt::Key_S:
                qDebug() << "key 's' pressed";
                m_backwardPressed = true;
                break;
            case Qt::Key_D:
                qDebug() << "key 'd' pressed";
                m_rightPressed = true;
                break;
            case Qt::Key_Q:
                qDebug() << "key 'q' pressed";
                m_worker->setCannon(true);
                break;
            case Qt::Key_E:
                qDebug() << "key 'e' pressed";
                emit actionLaserToggle(); // Used in UI to check/uncheck tickbox
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
    if(!k->isAutoRepeat()) {
        switch(k->key()) {
            case Qt::Key_W:
                qDebug() << "key 'w' released";
                m_forwardPressed = false;
                break;
            case Qt::Key_A:
                qDebug() << "key 'a' released";
                m_leftPressed = false;
                break;
            case Qt::Key_S:
                qDebug() << "key 's' released";
                m_backwardPressed = false;
                break;
            case Qt::Key_D:
                qDebug() << "key 'd' released";
                m_rightPressed = false;
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

InputType MainWindow::strToInputType(QString str) {
    if(str == "keyboardInput") {
        return KeyboardInput;
    }
    else if (str == "mouseInput") {
        return MouseInput;
    }
    else {
        return KeyboardInput;
    }
}