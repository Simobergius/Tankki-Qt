#ifndef BLUETOOTHWORKER_H
#define BLUETOOTHWORKER_H

#include <QObject>
#include <QBluetoothSocket>
#include <QTimerEvent>

class BluetoothWorker : public QObject 
{
    Q_OBJECT
public:
    BluetoothWorker(int sendIntervalMs);
    virtual ~BluetoothWorker();

    void stopRequested();

    void setSocket(QBluetoothSocket *socket);

    void setSpeedForwardBackward(int value);
    void setSpeedLeftRight(int value);
    void setMG(bool mg);
    void setCannon(bool cannon);
    void setLaser(bool laser);

    quint8 getLeftTrackSpeed();
    quint8 getRightTrackSpeed();

public slots:
    void onThreadFinished();
    void onThreadStarted();

protected:
    virtual void timerEvent(QTimerEvent* e);

private:
    qint8 mapToTracks(int x, int y);

    int m_timerId;
    int m_sendIntervalMs;
    bool m_stopRequested;
    QBluetoothSocket* m_socket;

    int m_speedForwardBackward, m_speedLeftRight;
    quint8 m_rightTrackSpeed, m_leftTrackSpeed;
    bool m_shootCannon, m_laser, m_shootMg;
};

#endif // BLUETOOTHWORKER_H