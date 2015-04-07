#include "mainwindow.h"
#include <QApplication>
#include <QNetworkAccessManager>
#include <QUrl>
#include <QNetworkRequest>

#include "ipfs.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();


    Ipfs ipfs;
    ipfs.test();

    return a.exec();
}
