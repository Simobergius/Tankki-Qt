#include "bluetoothworker.h"
#include "math_utils.h"

#include <QDebug>
#include <QByteArray>
#include <cmath>

// Define limits of track-speed values that will be sent to remote tank
const int toLow = 0, toHigh = 254;

// Define limit of application values
const int fromLow = -127, fromHigh = 127;

BluetoothWorker::BluetoothWorker(int sendIntervalMs) :
    m_timerId(-1), m_sendIntervalMs(sendIntervalMs), m_stopRequested(false), m_socket(NULL), \
    m_speedForwardBackward(0), m_speedLeftRight(0), m_rightTrackSpeed(127), m_leftTrackSpeed(127), \
    m_shootCannon(false), m_laser(false), m_shootMg(false)
{
}

BluetoothWorker::~BluetoothWorker() {

}

void BluetoothWorker::stopRequested() {
    m_stopRequested = true;
}

void BluetoothWorker::setSocket(QBluetoothSocket *socket) {
    m_socket = socket;
    if(m_stopRequested){
        m_stopRequested = false;
    }
}

void BluetoothWorker::setSpeedForwardBackward(int value) {
    m_speedForwardBackward = value;
    m_leftTrackSpeed = mapToTracks(-m_speedLeftRight, m_speedForwardBackward);
    m_rightTrackSpeed = mapToTracks(m_speedLeftRight, m_speedForwardBackward);
}

void BluetoothWorker::setSpeedLeftRight(int value) {
    m_speedLeftRight = value;
    m_leftTrackSpeed = mapToTracks(-m_speedLeftRight, m_speedForwardBackward);
    m_rightTrackSpeed = mapToTracks(m_speedLeftRight, m_speedForwardBackward);
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

quint8 BluetoothWorker::getLeftTrackSpeed() {
    return m_leftTrackSpeed;
}

quint8 BluetoothWorker::getRightTrackSpeed() {
    return m_rightTrackSpeed;
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
        // current setup accepts this format:
        // { 1, 255, right, left, elevation, rotation, cannon, laser, MG }
        // total of 9 bytes
        // 1,255 are magic numbers
        // right is 127 centered power of right track
        // left is same for left track
        // elevation is 127 centered desired elevation of cannon from 0 to 254
        // rotation is 127 centered turret rotation speed from 0 to 254
        // cannon, laser and MG are 2 if 'on' and 0 if 'off'
        bytesToWrite.append(0x01);
        bytesToWrite.append(0xFF);
        bytesToWrite.append(m_rightTrackSpeed);
        bytesToWrite.append(m_leftTrackSpeed);
        bytesToWrite.append(127); // TODO: Implement turret elevation
        bytesToWrite.append(127); // TODO: Implement turret rotation
        bytesToWrite.append(m_shootCannon ? 2 : 0);
        bytesToWrite.append(m_laser ? 2 : 0);
        bytesToWrite.append(m_shootMg ? 2 : 0);
        qDebug() << "Writing bytes: " << QString(bytesToWrite.toHex()) \
                 << ", forward-backward speed: " << QString::number(m_speedForwardBackward) \
                 << ", left-right speed: " << QString::number(m_speedLeftRight) \
                 << ", right track speed: " << QString::number(m_rightTrackSpeed) \
                 << ", left track speed: " << QString::number(m_leftTrackSpeed);
        
        if(m_socket->write(bytesToWrite) == -1) {
            qDebug() << "Write error";
        }
    }
}

qint8 BluetoothWorker::mapToTracks(int x, int y) {
    qint8 val = 0;

    if(y >= 0) {
        if (x > 0) {
            val = y-x;
        }
        else if(x <= 0) {
            val = (qint8) MathUtils::max(abs(x), abs(y));
        }
    }
    else if (y < 0) {
        if(x > 0) {
            if (abs(x) > abs(y)) {
                val = - x - y;
            }
            else if (abs(x) <= abs(y)) {
                val = x + y;
            }
        }
        else if (x <= 0) {
            if (abs(x) <= abs(y)) {
                val = y;
            }
            else if(abs(x) > abs(y)) {
                val = -(-x + 2 * y);
            }
        }
    }
    val = MathUtils::map(val, fromLow, fromHigh, toLow, toHigh);
    return val;
}