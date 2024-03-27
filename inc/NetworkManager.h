#ifndef NETWORKMANANGER_H
#define NETWORKMANANGER_H

#include <QNetworkInterface>
#include <QHash>
#include "NetworkInterface.h"


/**
 * @brief Manages the host interfaces on the network.
*/
class NetworkManager{
public:
    NetworkManager();
    ~NetworkManager();
    
private:

    /**
     * @brief Discover interfaces on the local system.
     * Similar to the output of `ifconfig` or `ipconfig`
    */
    void setLocalHostInterfaces();

    /**
     * @brief Setter for host interfaces.
    */
    void setHostInterfaces();

    /**
     * @brief Discover hosts interfaces on the given network.
     * Similar to `nmap 192.168.0.0/24`
    */
    QList<NetworkInterface> discoverHostInterfaces(NetworkInterface interface);

    /**
     * @brief Helper function to parse the nmap response & return a list of hosts IP address.
    */
    QList<QHostAddress> parseNmapResp(QString resp);


    /**
     * @brief List of local system's network interfaces like Wifi, ethernet, etc.
    */
    QList<NetworkInterface> m_localHostInterfaces;

    /**
     * @brief List of host interfaces found on each local interface's network 
     * e.g. hosts/devices on wifi or ethernet network.
     * 
     * The structure of data is 
     * {
     *  // Hosts/devices found on the wifi's main network.
     *  "wifiInterface": {device1Interface, device2Interface....},
     * 
     *  // Hosts/devices found on the ethernet's main network.
     *  "ethernetInterface": {device10Interface, device11Interface...},
     *  ...
     * }
    */
    QHash<NetworkInterface, QList<NetworkInterface>> m_hostInterfaces;
};

#endif