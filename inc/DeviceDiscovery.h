#ifndef DEVICEDISCOVERY_H
#define DEVICEDISCOVERY_H

#include <QNetworkInterface>
#include <QHostAddress>
#include <QSharedPointer>
#include <QProcess>
#include <QRegularExpression>
#include "DeviceInterface.h"

//!< \brief Discovers the device connected on the given interface network(ethernet or Wifi).
class DeviceDiscovery : public QObject
{
    Q_OBJECT
public:
    DeviceDiscovery(QSharedPointer<DeviceInterface> interface, QObject *parent = nullptr);
    ~DeviceDiscovery();

private:
    //!< \brief The interface on which devices would be discovered.
    QSharedPointer<DeviceInterface> m_interface;

    //!< \brief Helper function to parse the nmap response & return a list of device IP addresses on localhost network.
    QList<QHostAddress> parseNmapResp(QString);

signals:
    //!< \brief Signal emitting the discovered devices on the localhost network.
    void sig_discoveredDevicesOnNetwork(QList<QSharedPointer<DeviceInterface>>);

public slots:
    //!< \brief Runs the nmap process for discovering devices on the localhost network.
    void slot_startDeviceDiscovery();
};

#endif