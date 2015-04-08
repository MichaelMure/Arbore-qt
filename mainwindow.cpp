#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "ipfs/ipfs.h"

#include <QDebug>
#include <QString>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    refreshTimer_(this),
    downloadModel_(this)
{
    ui->setupUi(this);

    connect(&refreshTimer_, SIGNAL (timeout ()),
            this, SLOT(refresh()));
    refreshTimer_.start(1000);

    ui->downloadListView->setModel(&downloadModel_);

    refresh();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::refresh()
{
    ui->peerslabel->setText(
        QString::number(Ipfs::instance().swarm.peers().count()) + " peers");
}
