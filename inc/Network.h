#ifndef NETWORK_H
#define NETWORK_H

#include <QNetworkInterface>

class NetworkInterface{
public:
    QString m_networkCIDR;
    QNetworkInterface m_networkInterface;

    NetworkInterface(QString networkCIDR, QNetworkInterface networkInterface) :
    m_networkCIDR(networkCIDR),
    m_networkInterface(networkInterface)
    {}
};

class Network{
public:
    Network();
    ~Network();
    
private:
    
    QString getCIDRAddress(QHostAddress ipAddress, QHostAddress subnetMask);
    QList<NetworkInterface> getLocalHostInterfaces();
    QList<NetworkInterface> getInterfaces(QString cidrAddress);
    
};

#endif