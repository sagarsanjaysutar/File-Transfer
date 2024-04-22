#include "NetworkManager.h"
#include "ReceivingServer.h"

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QTextStream>
#include <QFile>
#include <QMap>
#include <QJsonObject>

int main(int argc, char *argv[]){

    // Standard Qt/QML Configuration
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:qml/main.qml")));

    // NetworkManager discovery;
    // ReceivingServer receive(discovery.getLocalHostInterface());

    // Expose backend objects to QML
    QQmlContext *ctxt;
    QVector<QQmlContext::PropertyPair> qmlProperties;
    ctxt = engine.rootContext();
    qmlProperties.push_back(QQmlContext::PropertyPair{"discovery", QVariant::fromValue(&discovery)});
    ctxt->setContextProperties(qmlProperties);


    return app.exec();
}
