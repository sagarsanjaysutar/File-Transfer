#ifndef NETWORKINTERFACE_H
#define NETWORKINTERFACE_H

#include <QHostAddress>
#include <QNetworkInterface>
#include <QString>
#include <QObject>

/**
 * @brief Represents a Network Interface e.g. a local computer on the network.
*/
class NetworkInterface : public QObject {
    Q_OBJECT
public:
    
    NetworkInterface(QObject* parent = nullptr);
    NetworkInterface(QHostAddress ip, QHostAddress mask, QString name = "", QNetworkInterface::InterfaceType type = QNetworkInterface::Unknown, QObject* parent = nullptr);
    NetworkInterface(const NetworkInterface &interface, QObject* parent = nullptr);

    // Getters
    QHostAddress getGatewayAddress() const { return m_gatewayAddress; }
    QHostAddress getIpAddress() const { return m_ipAddress; }
    Q_INVOKABLE QString getIpAddressStr() const { return m_ipAddress.toString(); }
    QHostAddress getMaskAddress() const { return m_maskAddress; }
    QString getCIDRAddress() const { return m_CIDRAddress; }
    QString getName() const { return m_name; }
    QNetworkInterface::InterfaceType getType() const { return m_type; }

private:

    // Setters
    void setGatewayAddress(const QHostAddress ipAddress);
    void setIpAddress(const QHostAddress ipAddress);
    void setMaskAddress(const QHostAddress maskAddress);
    void setCIDRAddress(QHostAddress ipAddress, QHostAddress mask);
    void setName(const QString name);
    
    QHostAddress m_gatewayAddress;              // Network Address. For 192.168.0.168 -> 192.168.0.1
    QHostAddress m_ipAddress;                   // IP Address
    QHostAddress m_maskAddress;                 // Netmask address
    QString m_CIDRAddress;                      // Network address: 192.168.1.0/24
    QString m_name;                             // Interface name
    QNetworkInterface::InterfaceType m_type;    // Interface type: Wifi, ethernet, etc.

signals:
    void sig_nameChanged();
};
Q_DECLARE_METATYPE(NetworkInterface)

#endif