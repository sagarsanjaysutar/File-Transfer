#ifndef DEVICEINTERFACE_H
#define DEVICEINTERFACE_H

#include <QHostAddress>
#include <QNetworkInterface>
#include <QString>
#include <QObject>

//!< \brief A network interface representing a device connected to the ethernet or Wifi network.
class DeviceInterface : public QObject
{
    Q_OBJECT
public:
    DeviceInterface(QObject *parent = nullptr);
    DeviceInterface(QHostAddress ip, QHostAddress mask, QString name = "", QNetworkInterface::InterfaceType type = QNetworkInterface::Unknown, QObject *parent = nullptr);
    DeviceInterface(const DeviceInterface &interface, QObject *parent = nullptr);

    // Getters
    QHostAddress getIpAddress() const { return m_ipAddress; }
    Q_INVOKABLE QString getIpAddressStr() const { return m_ipAddress.toString(); }
    QHostAddress getMaskAddress() const { return m_maskAddress; }
    QString getName() const { return m_name; }
    QNetworkInterface::InterfaceType getType() const { return m_type; }

    QHostAddress getGatewayAddress() const { return m_gatewayAddress; }
    QString getCIDRAddress() const { return m_CIDRAddress; }

private:
    //!< \brief Calculates the gateway address using the IP & netmask.
    void setGatewayAddress(const QHostAddress ipAddress, const QHostAddress mask);

    //!< \brief Calculates the CIDR address using the IP & netmask.
    void setCIDRAddress(const QHostAddress ipAddress, const QHostAddress mask);

    // Member Variables
    QHostAddress m_ipAddress;                // IP Address e.g 192.168.0.162
    QHostAddress m_maskAddress;              // Mask address
    QString m_name;                          // Interface name
    QNetworkInterface::InterfaceType m_type; // Interface type: Wifi, ethernet, etc.

    /** \brief Gateway Address
     * Typically the first address in a network range (after the network address)
     * e.g 192.168.0.1 is the gateway address of 192.168.0.162 with a subnet 255.255.255.0.
     */
    QHostAddress m_gatewayAddress;

    /** \brief CIDR network address: Represents an IP ranges. Looks like this `IP_address/prefix_length`.
     * e.g. 192.168.0.0/24 is a CIDR address of 192.168.0.162 with a subnet 255.255.255.0.
     * It represents the IP ranges from 192.168.0.0 to 192.168.0.255.
     * \ref https://blog.ip2location.com/knowledge-base/how-to-convert-ip-address-range-into-cidr/
     */
    QString m_CIDRAddress;

signals:
    void sig_nameChanged();
};

Q_DECLARE_METATYPE(DeviceInterface)

#endif