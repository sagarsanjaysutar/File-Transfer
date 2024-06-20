#ifndef NETWORKMANANGER_H
#define NETWORKMANANGER_H

#include <QNetworkInterface>
#include <QSharedPointer>
#include <QVariant>
#include <QJsonObject>

#include "DeviceInterface.h"
#include "DiscoveryThread.h"

/**
 * \brief Manages the devices on localhost network.
*/
class NetworkManager : public QObject {
    Q_OBJECT
    Q_PROPERTY(QVariantList devicesOnNetwork READ getDevicesOnNetwork NOTIFY sig_devicesOnNetworkUpdated)
    
public:
    NetworkManager(QObject *parent = nullptr);
    ~NetworkManager();

    //!< \brief Start discovery process for devices on localhost network.
    Q_INVOKABLE void startDeviceDiscovery();

    //!< \brief Returns a QVariantList of m_devicesOnNetwork.
    QVariantList getDevicesOnNetwork();

private:
    //!< \brief A thread managing the device discovery process on the localhost network.
    DiscoveryThread *m_discoveryThread;

    //!< \brief List of devices on the localhost network.
    QList<QSharedPointer<DeviceInterface>> m_devicesOnNetwork;

    //!< \brief DeviceInterface representing the localhost/local computer.
    QSharedPointer<DeviceInterface> m_localHostInterface;

signals:
    void sig_devicesOnNetworkUpdated();
};

#endif