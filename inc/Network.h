#ifndef NETWORK_H
#define NETWORK_H

#include <QNetworkInterface>
#include "NetworkInterface.h"


class Network{
public:
    Network();
    ~Network();
    
private:
    QList<NetworkInterface> m_localHostInterfaces;
    QList<NetworkInterface> getLocalHostInterfaces();

    /**
     * @brief Get list of SFTP hosts on the network interface.
    */
    QList<NetworkInterface> getSFTPInterfaces(NetworkInterface interface);
    QList<QHostAddress> parseNmapResp(QString resp);
    
};

#endif