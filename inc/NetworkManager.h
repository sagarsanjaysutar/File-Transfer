#ifndef NETWORKMANANGER_H
#define NETWORKMANANGER_H

#include <QNetworkInterface>
#include <QSharedPointer>
#include <QVariant>
#include <QJsonObject>

#include "NetworkInterface.h"
#include "DiscoveryThread.h"

/**
 * @brief Manages the host interfaces on the network.
*/
class NetworkManager : public QObject {
    Q_OBJECT
    Q_PROPERTY(QVariantList hosts MEMBER m_hostInterfaces NOTIFY sig_hostsUpdated)
    
public:
    NetworkManager(QObject *parent = nullptr);
    ~NetworkManager();

    //!< \brief Set & Discover hosts interfaces on the localhost network.
    Q_INVOKABLE void initHostInterfaces();

    QVariantList m_hostInterfaces;

private:
    DiscoveryThread *m_discoveryThread;

signals:
    void sig_hostsUpdated();
};

#endif