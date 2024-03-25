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

void NetworkInterface::setCIDRAddress(QHostAddress ipAddress, QHostAddress subnetMask){
    
    // Convert the IP Address into integer.
    quint32 subnetAddrInt = subnetMask.toIPv4Address();
    
    // Initialize the CIDR notation
    int cidr = 0;
    
    // Count the number of "1" in the subnet mask
    while(subnetAddrInt){
        cidr = cidr + (subnetAddrInt & 1);
        subnetAddrInt = subnetAddrInt >> 1;
    }

    QStringList ipAddrList = ipAddress.toString().split(".");
    ipAddrList[ipAddrList.count() - 1] = "0";
    m_CIDRAddress = ipAddrList.join(".").append("/").append(QString::number(cidr));
}