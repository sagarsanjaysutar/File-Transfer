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
    NetworkInterface(QHostAddress ip, QHostAddress mask);
    NetworkInterface(QNetworkInterface interface);

    /**
     * @brief Since this class is used a key in a QHash, it requires == override. 
    */
    bool operator == (const NetworkInterface& rhs) const { return m_ipAddress == rhs.getIpAddress(); }

    // Getters
    QString getCIDRAddress() const { return m_CIDRAddress; }
    QHostAddress getIpAddress() const { return m_ipAddress; }
    QHostAddress getMaskAddress() const { return m_maskAddress; }
    QString getName() const { return m_name; }

private:

    // Setters
    void setCIDRAddress(QHostAddress ipAddress, QHostAddress mask);
    void setIpAddress(const QHostAddress ipAddress);
    void setMaskAddress(const QHostAddress maskAddress);
    void setName(const QString name);

    
    QString m_CIDRAddress;      // Network address: 192.168.1.0/24
    QHostAddress m_ipAddress;   // IP Address
    QHostAddress m_maskAddress; // Netmask address
    QString m_name;             // Interface name
};

/**
 * @brief Since this class is used a key in a QHash, it requires a global hash function.
*/
inline uint qHash(const NetworkInterface &rhs) {return qHash(rhs.getCIDRAddress()); }

#endif