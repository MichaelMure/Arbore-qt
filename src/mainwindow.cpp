#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "ipfs/ipfs.h"

#include <QDebug>
#include <QString>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    refreshTimer_(this),
    shareModel_(this),
    shareDelegate_(this)
{
    ui->setupUi(this);

    connect(&refreshTimer_, SIGNAL (timeout ()),
            this, SLOT(refresh()));
    refreshTimer_.start(1000);

    ui->downloadListView->setModel(&shareModel_);
    ui->downloadListView->setItemDelegate(&shareDelegate_);

    refresh();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::refresh()
{
    ui->peerslabel->setText(
        QString::number(Ipfs::instance().swarm.peer_count()) + " peers");
    ui->statslabel->setText(Ipfs::instance().stats.ToString());
}
