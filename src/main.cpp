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

    /*Ipfs::instance().pin.ls_pin();

    IpfsHash hash = IpfsHash("QmPXME1oRtoT627YKaDPDQ3PwA8tdP9rWuAAweLzqSwAWT");
    Ipfs::instance().pin.add_pin(hash);*/

    return a.exec();
}
