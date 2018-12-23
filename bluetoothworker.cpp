#include "bluetoothworker.h"

#include <QDebug>
#include <QByteArray>
#include <cmath>

// Define limits of track-speed values that will be sent to remote tank
const int toLow = 0, toHigh = 255;

// Define limit of application values
const int fromLow = -128, fromHigh = 127;

BluetoothWorker::BluetoothWorker(int sendIntervalMs) :
    m_timerId(-1), m_sendIntervalMs(sendIntervalMs), m_stopRequested(false), m_socket(NULL), \
    m_speedForwardBackward(0), m_speedLeftRight(0), m_shootCannon(false), m_laser(false), m_shootMg(false)
{
}

BluetoothWorker::~BluetoothWorker() {

}

void BluetoothWorker::stopRequested() {
    m_stopRequested = true;
}

void BluetoothWorker::setSocket(QBluetoothSocket *socket) {
    m_socket = socket;
}

void BluetoothWorker::setSpeedForwardBackward(int value) {
    m_speedForwardBackward = value;
}

void BluetoothWorker::setSpeedLeftRight(int value) {
    m_speedLeftRight = value;
}

void BluetoothWorker::setCannon(bool cannon) {
    m_shootCannon = cannon;
}

void BluetoothWorker::setMG(bool mg) {
    m_shootMg = mg;
}

void BluetoothWorker::setLaser(bool laser) {
    m_laser = laser;
}

void BluetoothWorker::onThreadFinished() {
    killTimer(m_timerId);
    m_timerId = -1;
}

void BluetoothWorker::onThreadStarted() {
    m_timerId = startTimer(m_sendIntervalMs);
}

void BluetoothWorker::timerEvent(QTimerEvent *e) {
    if(m_stopRequested) {
            qDebug() << "Stop requested, kill timer";
            onThreadFinished();
    }

    if(e->timerId() == m_timerId && m_socket) {
        // Send bytes to remote tank
        QByteArray bytesToWrite;
        // TODO: Determine bytes to send, current setup accepts this format:
        // { 1, 255, right, left, elevation, rotation, cannon, laser, MG }
        // total of 9 bytes
        // 1,255 are maic numbers
        // right is 127 centered power of right track
        // left is same for left track
        // elevation is 127 centered desired elevation of cannon from 0 to 254
        // rotation is 127 centered turret rotation speed from 0 to 254
        // cannon, laser and MG are 2 if 'on' and 0 if 'off'
        if(m_socket->write(bytesToWrite) == -1) {
            qDebug() << "Write error";
        }
    }
}

qint8 BluetoothWorker::mapToTracks(int x, int y) {
    qint8 val = 0;

    if(y <= 0) {
        if (x > 0) {
            val = -y-x;
        }
        else if(x <= 0) {
            if(abs(x) >= abs(y)) {
                val = abs(x);
            }
            else if(abs(x) < abs(y)) {
                val = abs(y);
            }
        }
    }else if (y > 0) {
        if(x > 0) {
            if (x > y) {
                val = y - x;
            }
            else if (x < y) {
                val = x - y;
            }
        }
        else if (x <= 0) {
            if (x <= y) {
                val = -y;
            }
            else if(x > y) {
                val = -(x + 2 * y);
            }
        }
    }

    val = (val - fromLow) * (toHigh - toLow) / (fromHigh - fromLow) + toLow;

    return val;
}