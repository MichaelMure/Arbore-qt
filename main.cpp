#include "mainwindow.h"
#include "ipfs/ipfs.h"
#include <QApplication>
#include <QDebug>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();


    /*ipfs ipfs;
    ipfs.test();*/

    qDebug() << Ipfs::instance().version.ToString();

    return a.exec();
}
