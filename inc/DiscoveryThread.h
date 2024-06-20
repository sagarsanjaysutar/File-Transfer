#ifndef DISCOVERYTHREAD_H
#define DISCOVERYTHREAD_H

#include <QNetworkInterface>
#include <QHostAddress>
#include <QSharedPointer>
#include <QThread>
#include <QProcess>
#include <QRegularExpression>
#include "DeviceInterface.h"

//!< \brief Discovers the device interfaces connected to the ethernet or Wifi network.
class DiscoveryThread : public QThread{

    Q_OBJECT

public:

    DiscoveryThread(QObject *parent);

private:

    //!< \brief Runs the nmap process for discovering devices on the localhost network.
    void run() override;

    //!< \brief Returns the list of interfaces on a localhost/local computer.
    QList<QSharedPointer<DeviceInterface>> getLocalHostInterfaces();

    //!< \brief Helper function to parse the nmap response & return a list of device IP addresses on localhost network.
    QList<QHostAddress> parseNmapResp(QString);

signals: 

    //!< \brief Localhost interface.
    void sig_detectedLocalHostInterface(QSharedPointer<DeviceInterface>);

    //!< \brief Signal emitting the discovered devices on the localhost network.
    void sig_discoveredDevicesOnNetwork(QList<QSharedPointer<DeviceInterface>>);
};

#endif