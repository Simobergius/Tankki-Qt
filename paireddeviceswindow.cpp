#include "paireddeviceswindow.h"
#include "ui_paireddeviceswindow.h"

PairedDevicesWindow::PairedDevicesWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PairedDevicesWindow)
{
    ui->setupUi(this);
}

PairedDevicesWindow::~PairedDevicesWindow()
{
    delete ui;
}
