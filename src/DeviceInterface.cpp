#include "DeviceInterface.h"

DeviceInterface::DeviceInterface(QObject *parent) : QObject(parent),
                                                    m_gatewayAddress(""),
                                                    m_ipAddress(""),
                                                    m_maskAddress(""),
                                                    m_CIDRAddress(""),
                                                    m_name(""),
                                                    m_type(QNetworkInterface::Unknown)
{
    printInterfaceInfo();
}

DeviceInterface::DeviceInterface(QHostAddress ip, QHostAddress mask, QString name, QNetworkInterface::InterfaceType type, QObject *parent) : QObject(parent),
                                                                                                                                             m_gatewayAddress(""),
                                                                                                                                             m_ipAddress(ip),
                                                                                                                                             m_maskAddress(mask),
                                                                                                                                             m_CIDRAddress(""),
                                                                                                                                             m_name(name),
                                                                                                                                             m_type(type)
{
    setGatewayAddress(m_ipAddress, m_maskAddress);
    setCIDRAddress(m_ipAddress, m_maskAddress);
    printInterfaceInfo();
}

DeviceInterface::DeviceInterface(const DeviceInterface &interface, QObject *parent) : QObject(parent),
                                                                                      m_gatewayAddress(interface.getGatewayAddress()),
                                                                                      m_ipAddress(interface.getIpAddress()),
                                                                                      m_maskAddress(interface.getMaskAddress()),
                                                                                      m_CIDRAddress(interface.getCIDRAddress()),
                                                                                      m_name(interface.getName()),
                                                                                      m_type(interface.getType())
{
    printInterfaceInfo();
}

void DeviceInterface::printInterfaceInfo()
{
    qDebug() << "DeviceInterface: Interface Info: IP Address: " << m_ipAddress
             << " Mask Address: " << m_maskAddress
             << " Gateway Address: " << m_gatewayAddress
             << " CIDR Address: " << m_CIDRAddress;
}

void DeviceInterface::setGatewayAddress(const QHostAddress ipAddress, const QHostAddress subnetMask)
{

    if (ipAddress.isNull())
    {
        qDebug() << "DeviceInterface: Can't set Gateway address, Invalid IP.";
        return;
    }
    else
    {
        // Perform a bitwise AND b/w IP address & mask address to get the network address.
        // Gateway address is network address + 1.
        m_gatewayAddress = QHostAddress((ipAddress.toIPv4Address() & subnetMask.toIPv4Address()) + 1);
        // qDebug() << "DeviceInterface: Gateway address set to " << m_gatewayAddress;
    }
}

void DeviceInterface::setCIDRAddress(const QHostAddress ipAddress, const QHostAddress subnetMask)
{

    if (ipAddress.isNull() || subnetMask.isNull())
    {
        qDebug() << "DeviceInterface: Can't set CIDR address, Invalid IP & Mask.";
        return;
    }

    // Convert the subnet address into integer.
    quint32 subnetAddrInt = subnetMask.toIPv4Address();

    // This is the correct way to determine the CIDR prefer-length
    // Initialize the CIDR prefix-length
    int cidr = 0;

    // Count the number of "1" in the subnet mask
    while (subnetAddrInt)
    {
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

    // qDebug() << "DeviceInterface: CIDR address set to " << m_CIDRAddress;
}