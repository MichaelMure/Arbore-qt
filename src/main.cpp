#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtQml>

#include "sharemodel.h"
#include "share.h"
#include "persist/persist.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    qmlRegisterType<Share>("Arbore", 1, 0, "Share");

    QQmlApplicationEngine engine;

    ShareModel shareModel;
    engine.rootContext()->setContextProperty("shareModel", &shareModel);

    engine.load(QUrl(QStringLiteral("qrc:/mainWindow.qml")));

    return app.exec();
}
