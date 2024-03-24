#include "NetworkInterface.h"

NetworkInterface::NetworkInterface(QHostAddress ip, QHostAddress mask) : 
    m_CIDRAddress(""),
    m_ipAddress(ip),
    m_maskAddress(mask),
    m_name("")
{
    // Set CIDR Address
    setCIDRAddress(m_ipAddress, m_maskAddress);
}

NetworkInterface::NetworkInterface(QNetworkInterface networkInterface) :
    m_CIDRAddress(""),
    m_ipAddress(""),
    m_maskAddress(""),
    m_name(networkInterface.name())
{
    // Set IP & subnet mask address
    for(QNetworkAddressEntry entry : networkInterface.addressEntries()){
        if(entry.ip().protocol() == QAbstractSocket::IPv4Protocol){
            m_ipAddress = entry.ip();
            m_maskAddress = entry.netmask();
            break;
        }
    }

    // Set CIDR Address
    setCIDRAddress(m_ipAddress, m_maskAddress);
}