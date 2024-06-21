#ifndef DEVICEINTERFACE_H
#define DEVICEINTERFACE_H

#include <QHostAddress>
#include <QNetworkInterface>
#include <QString>
#include <QObject>

//!< \brief A network interface representing a device connected to the ethernet or Wifi network.
class DeviceInterface : public QObject {
    Q_OBJECT
public:
    
    DeviceInterface(QObject* parent = nullptr);
    DeviceInterface(QHostAddress ip, QHostAddress mask, QString name = "", QNetworkInterface::InterfaceType type = QNetworkInterface::Unknown, QObject* parent = nullptr);
    DeviceInterface(const DeviceInterface &interface, QObject* parent = nullptr);

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
    
    // Variables
    QHostAddress m_gatewayAddress;              // Gateway Address. e.g Gateway of 192.168.0.162 is 192.168.0.1
    QHostAddress m_ipAddress;                   // IP Address e.g 192.168.0.162
    QHostAddress m_maskAddress;                 // Mask address
    QString m_CIDRAddress;                      // CIDR Network address: e.g. CIDR Address of 192.168.0.162 is 192.168.0.0/24
    QString m_name;                             // Interface name
    QNetworkInterface::InterfaceType m_type;    // Interface type: Wifi, ethernet, etc.

signals:
    void sig_nameChanged();
};
Q_DECLARE_METATYPE(DeviceInterface)

#endif