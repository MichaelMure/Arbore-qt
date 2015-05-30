#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "sharemodel.h"



int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    ShareModel shareModel;
    engine.rootContext()->setContextProperty("shareModel", &shareModel);

    engine.load(QUrl(QStringLiteral("qrc:/mainWindow.qml")));

    return app.exec();
}
