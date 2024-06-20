#include "DeviceInterface.h"


DeviceInterface::DeviceInterface(QObject* parent) :
    QObject(parent),
    m_gatewayAddress(""),
    m_ipAddress(""),
    m_maskAddress(""),
    m_CIDRAddress(""),
    m_name(""),
    m_type(QNetworkInterface::Unknown)
{
}

DeviceInterface::DeviceInterface(QHostAddress ip, QHostAddress mask, QString name, QNetworkInterface::InterfaceType type, QObject* parent) : 
    QObject(parent),
    m_gatewayAddress(""),
    m_ipAddress(ip),
    m_maskAddress(mask),
    m_CIDRAddress(""),
    m_name(name),
    m_type(type)
{
    setGatewayAddress(m_ipAddress);
    setCIDRAddress(m_ipAddress, m_maskAddress);
}

DeviceInterface::DeviceInterface(const DeviceInterface &interface, QObject* parent) :
    QObject(parent),
    m_gatewayAddress(interface.getGatewayAddress()),
    m_ipAddress(interface.getIpAddress()),
    m_maskAddress(interface.getMaskAddress()),
    m_CIDRAddress(interface.getCIDRAddress()),
    m_name(interface.getName()),
    m_type(interface.getType())
{}

void DeviceInterface::setGatewayAddress(const QHostAddress ipAddress){

    if(ipAddress.isNull()){
        qDebug() << "DeviceInterface: Can't set Gateway address, Invalid IP.";
        return;
    }
    else{
        // Build the Gateway address from IP address:
        // e.g. 192.168.0.192 -> 192.168.0.1
        QStringList ipAddrList = ipAddress.toString().split(".");

        // Replace the host number of the IP address with 1.
        ipAddrList[ipAddrList.count() - 1] = "1";

        m_gatewayAddress = QHostAddress(ipAddrList.join("."));
    }
}

void DeviceInterface::setCIDRAddress(QHostAddress ipAddress, QHostAddress subnetMask){

    if(ipAddress.isNull() || subnetMask.isNull()){
        qDebug() << "DeviceInterface: Can't set CIDR address, Invalid IP & Mask.";
        return;
    }
    
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

    // Replace the host ID of the IP address (last part) with 0.
    ipAddrList[ipAddrList.count() - 1] = "0";

    // Append the CIDR number to the IP.
    m_CIDRAddress = ipAddrList.join(".").append("/").append(QString::number(cidr));
}

void DeviceInterface::setIpAddress(const QHostAddress ipAddress) {
    if(m_ipAddress != ipAddress)
        m_ipAddress = ipAddress;
}

void DeviceInterface::setMaskAddress(const QHostAddress maskAddress) {
    if(m_maskAddress != maskAddress)
        m_maskAddress = maskAddress;
}

void DeviceInterface::setName(const QString name) {
    if(m_name != name)
        m_name = name;
}