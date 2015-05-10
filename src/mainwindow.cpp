#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "ipfs/ipfs.h"

#include <QDebug>
#include <QString>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    shareModel_(this),
    shareDelegate_(this)
{
    ui->setupUi(this);

    startTimer(1000); // 1s

    ui->downloadListView->setModel(&shareModel_);
    ui->downloadListView->setItemDelegate(&shareDelegate_);

    refresh();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::refresh() const
{
    ui->peerslabel->setText(
        QString::number(Ipfs::instance().swarm.peer_count()) + " peers");
    ui->statslabel->setText(Ipfs::instance().stats.ToString());
}

void MainWindow::timerEvent(QTimerEvent *)
{
    refresh();
}
