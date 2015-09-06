#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtQml>

#include "sharemodel.h"
#include "sharefilter.h"
#include "share.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    qmlRegisterType<Share>("Arbore", 1, 0, "Share");

    QQmlApplicationEngine engine;

    ShareModel shareModel;
    ShareFilter *shareFilter = new ShareFilter(&app);
    shareFilter->setSourceModel(&shareModel);
    engine.rootContext()->setContextProperty("shareModel", shareFilter);

    engine.load(QUrl(QStringLiteral("qrc:/mainWindow.qml")));

    return app.exec();
}
