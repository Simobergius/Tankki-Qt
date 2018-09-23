#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "paireddeviceswindow.h"

#include <QMainWindow>
#include <QPushButton>
#include <QKeyEvent>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
public slots:
signals:
    void actionForwardButtonClick();
    void actionForwardButtonRelease();
    void actionLeftButtonClick();
    void actionLeftButtonRelease();
    void actionBackwardButtonClick();
    void actionBackwardButtonRelease();
    void actionRightButtonClick();
    void actionRightButtonRelease();
    void actionMGOn();
    void actionMGOff();
    void actionCannonOn();
    void actionCannonOff();
    void actionLaserToggle();
    void actionListConnectedDevices();

private slots:
    void laserToggle(bool);
    void onCannonButtonClick();
    void onCannonButtonRelease();
    void onMGButtonClick();
    void onMGButtonRelease();
    void on_actionListConnectedDevices_triggered();

private:
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);

    Ui::MainWindow *ui;
    QStatusBar* m_StatusBar;
    PairedDevicesWindow *m_pairedDevicesWindow;
};

#endif // MAINWINDOW_H