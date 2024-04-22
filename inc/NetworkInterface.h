#ifndef NETWORKINTERFACE_H
#define NETWORKINTERFACE_H

#include <QHostAddress>
#include <QNetworkInterface>
#include <QString>

/**
 * @brief Represents a Network Interface e.g. a local computer on the network.
*/
class NetworkInterface {
public:
    NetworkInterface(QHostAddress ip, QHostAddress mask, QString name = "", QNetworkInterface::InterfaceType type = QNetworkInterface::Unknown);
    NetworkInterface(QNetworkInterface interface);

    // Getters
    QString getCIDRAddress() const { return m_CIDRAddress; }
    QHostAddress getIpAddress() const { return m_ipAddress; }
    QHostAddress getMaskAddress() const { return m_maskAddress; }
    QString getName() const { return m_name; }
    QNetworkInterface::InterfaceType getType() const { return m_type; }

private:

    // Setters
    void setCIDRAddress(QHostAddress ipAddress, QHostAddress mask);
    void setIpAddress(const QHostAddress ipAddress);
    void setMaskAddress(const QHostAddress maskAddress);
    void setName(const QString name);
    
    QString m_CIDRAddress;                      // Network address: 192.168.1.0/24
    QHostAddress m_ipAddress;                   // IP Address
    QHostAddress m_maskAddress;                 // Netmask address
    QString m_name;                             // Interface name
    QNetworkInterface::InterfaceType m_type;    // Interface type: Wifi, ethernet, etc.
};

#endif