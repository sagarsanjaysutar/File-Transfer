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
    m_name("")
{
    // Set IP & subnet mask address
    for(QNetworkAddressEntry entry : networkInterface.addressEntries()){
        if(entry.ip().protocol() == QAbstractSocket::IPv4Protocol){
            setIpAddress(entry.ip());
            setMaskAddress(entry.netmask());
            break;
        }
    }

    // Set CIDR Address
    setCIDRAddress(m_ipAddress, m_maskAddress);
    
    // Interface name
    setName(networkInterface.name());
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

    // Build the CIDR address from IP address:
    // e.g. 192.168.0.192 -> 192.168.0.0/24
    QStringList ipAddrList = ipAddress.toString().split(".");

    // Replace the host number of the IP address with 0.
    ipAddrList[ipAddrList.count() - 1] = "0";

    // Append the CIDR number to the IP.
    m_CIDRAddress = ipAddrList.join(".").append("/").append(QString::number(cidr));
}

void NetworkInterface::setIpAddress(const QHostAddress ipAddress) {
    if(m_ipAddress != ipAddress)
        m_ipAddress = ipAddress;
}

void NetworkInterface::setMaskAddress(const QHostAddress maskAddress) {
    if(m_maskAddress != maskAddress)
        m_maskAddress = maskAddress;
}

void NetworkInterface::setName(const QString name) {
    if(m_name != name)
        m_name = name;
}