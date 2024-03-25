#ifndef NETWORK_H
#define NETWORK_H

#include <QNetworkInterface>
#include "NetworkInterface.h"


/**
 * @brief 
*/
class Network{
public:
    Network();
    ~Network();
    
private:
    /**
     * @brief List of local interfaces.
    */
    QList<NetworkInterface> m_localHostInterfaces;

    /**
     * @brief Get a list of interfaces on the local machine.
     * Similar to `ifconfig` or `ipconfig`
    */
    QList<NetworkInterface> getLocalHostInterfaces();

    /**
     * @brief Get a list of active SFTP hosts/interfaces on the given network.
     * Similar to `nmap -p 22 192.168.0.0/24`
    */
    QList<NetworkInterface> getSFTPInterfaces(NetworkInterface interface);

    /**
     * @brief Helper function to parse the nmap response & return a list of hosts IP address.
    */
    QList<QHostAddress> parseNmapResp(QString resp);
};

#endif