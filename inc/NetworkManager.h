#ifndef NETWORKMANANGER_H
#define NETWORKMANANGER_H

#include <QNetworkInterface>
#include <QSharedPointer>
#include <QVariant>
#include <QJsonObject>
#include <QThread>

#include "DeviceInterface.h"
#include "DeviceDiscovery.h"

//!< \brief Manages the devices on localhost network.
class NetworkManager : public QObject {
    Q_OBJECT
    Q_PROPERTY(QVariantList devicesOnNetwork READ getDevicesOnNetwork NOTIFY sig_devicesOnNetworkUpdated)
    Q_PROPERTY(QString localHostIPAddress READ getLocalHostIPAddress CONSTANT)
    
public:
    NetworkManager(QObject *parent = nullptr);
    ~NetworkManager();

    //!< \brief Kicks off the QThread managing the device discovery process on the localhost network.
    Q_INVOKABLE void startDeviceDiscovery();

    //!< \brief Helper function that returns the first detected localhost.
    QSharedPointer<DeviceInterface> getLocalHostInterface();

    //!\brief Helper function that returns the IP Address of first detected localhost.
    QString getLocalHostIPAddress();

private:
    //!< \brief A QThread & worker managing the device discovery process on the localhost network.
    QThread *m_discoveryThread;
    DeviceDiscovery* m_deviceDiscoveryWorker;

    //!< \brief List of devices on the localhost network.
    QList<QSharedPointer<DeviceInterface>> m_devicesOnNetwork;

    //!< \brief Returns a QVariantList of m_devicesOnNetwork.
    QVariantList getDevicesOnNetwork();

    //!< \brief Returns the list of interfaces on a localhost/local computer.
    QList<QSharedPointer<DeviceInterface>> getLocalHostInterfaces();

    

signals:
    void sig_devicesOnNetworkUpdated();
};

#endif