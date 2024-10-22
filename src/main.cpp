#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QTextStream>
#include <QFile>
#include <QMap>
#include <QJsonObject>
#include <QStringList>

#include "NetworkManager.h"
#include "Sender.h"
#include "SFTPAccess.h"

int main(int argc, char *argv[])
{
    QStringList environment = QProcess::systemEnvironment();
    qDebug() << "Env: " << environment;
    // Standard Qt/QML Configuration
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

    NetworkManager discovery;
    SFTPAccess sftp(discovery.getLocalHostIPAddress());

    // Expose backend objects to QML
    QQmlContext *ctxt = engine.rootContext();
    QVector<QQmlContext::PropertyPair> qmlProperties;
    qmlProperties.push_back(QQmlContext::PropertyPair{"discovery", QVariant::fromValue(&discovery)});
    qmlProperties.push_back(QQmlContext::PropertyPair{"sftp", QVariant::fromValue(&sftp)});
    ctxt->setContextProperties(qmlProperties);

    engine.load(QUrl(QStringLiteral("qrc:qml/main.qml")));

    return app.exec();
}
