#include <QApplication>
#include <QMessageBox>
#include <QQmlApplicationEngine>
#include <QSystemTrayIcon>
#include <QtQml>

#include "sharemodel.h"
#include "sharefilter.h"
#include "share.h"

#include "ipfs/ipfs.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
  
    // We don't want to close the program when the main window is
    // closed
    //app.setQuitOnLastWindowClosed(false);
      
    qmlRegisterType<Share>("Arbore", 1, 0, "Share");
  
    QQmlApplicationEngine engine;
  
    ShareModel shareModel;
    ShareFilter *shareFilter = new ShareFilter(&app);
    shareFilter->setSourceModel(&shareModel);
    engine.rootContext()->setContextProperty("shareModel", shareFilter);
  
    engine.load(QUrl(QStringLiteral("qrc:/mainWindow.qml")));
    QObject *root = 0;
    if (engine.rootObjects().size() > 0)
    {
        root = engine.rootObjects().at(0);
        if (QSystemTrayIcon::isSystemTrayAvailable())
        {
//            QSystemTrayIcon *trayIcon = new QSystemTrayIcon(root);
//            trayIcon->setIcon(QIcon(":/arboreTray.png"));
//            trayIcon->show();
        }
    }

    /*Ipfs::instance()->object.new_object();
    Ipfs::instance()->object.add_link(IpfsHash("QmdfTbBqBPQ7VNxZEYEj14VmRuZBkqFbiwReogJgS1zR1n"),
                                      "bloh",
                                      IpfsHash("QmRFv66mJPKj55DpkrRMa29WgZHjLm8kRdndjf9Q9vKyJF"));*/
    //Ipfs::instance()->log.log_level_all(IpfsLog::ERROR);

    return app.exec();
}
