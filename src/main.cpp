#include "mainwindow.h"
#include "ipfs/ipfs.h"
#include <QApplication>
#include <QDebug>
#include <QTime>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
