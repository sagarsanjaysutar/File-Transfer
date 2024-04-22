#ifndef NETWORKMANANGER_H
#define NETWORKMANANGER_H

#include <QNetworkInterface>
#include "NetworkInterface.h"


/**
 * @brief Manages the host interfaces on the network.
*/
class NetworkManager{
public:
    NetworkManager();
    ~NetworkManager();

    
    QList<NetworkInterface> getLocalHostInterfaces() const {return m_localHostInterfaces; };
    NetworkInterface getLocalHostInterface();

    /**
     * @brief Discover hosts interfaces on the given network.
     * Similar to `nmap 192.168.0.0/24`
    */
    QList<NetworkInterface> getHostInterfaces(NetworkInterface interface);

private:

    /**
     * @brief List of local system's network interfaces like Wifi, ethernet, etc.
    */
    QList<NetworkInterface> m_localHostInterfaces;

    /**
     * @brief Discover interfaces on the local system.
     * Similar to the output of `ifconfig` or `ipconfig`
    */
    void setLocalHostInterfaces();

    /**
     * @brief Helper function to parse the nmap response & return a list of hosts IP address.
    */
    QList<QHostAddress> parseNmapResp(QString resp);
};

#endif