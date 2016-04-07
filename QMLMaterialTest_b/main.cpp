#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlComponent>
#include <QQmlContext>
#include <QVariant>
#include <QQmlProperty>
#include <iostream>
#include "windowadapter.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    WindowAdapter *windowadapter = new WindowAdapter;

    engine.rootContext()->setContextProperty("windowadapter", windowadapter);

    QQmlComponent component(&engine, QUrl::fromLocalFile("main.qml"));
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    QObject *object = component.create();



    return app.exec();
}
