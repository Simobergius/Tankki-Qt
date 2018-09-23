#ifndef PAIREDDEVICESWINDOW_H
#define PAIREDDEVICESWINDOW_H

#include <QDialog>

namespace Ui {
class PairedDevicesWindow;
}

class PairedDevicesWindow : public QDialog
{
    Q_OBJECT

public:
    explicit PairedDevicesWindow(QWidget *parent = 0);
    ~PairedDevicesWindow();

private:
    Ui::PairedDevicesWindow *ui;
};

#endif // PAIREDDEVICESWINDOW_H
