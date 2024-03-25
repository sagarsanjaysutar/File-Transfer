#ifndef NETWORKINTERFACE_H
#define NETWORKINTERFACE_H

#include <QHostAddress>
#include <QNetworkInterface>
#include <QString>
#include <QStringList>

// Think more on this. Inherit QNetworkInterface or own it.
class NetworkInterface {
public:
    NetworkInterface(QHostAddress ip, QHostAddress mask);
    NetworkInterface(QNetworkInterface interface);

    QString m_CIDRAddress;
    QHostAddress m_ipAddress;
    QHostAddress m_maskAddress;
    QString m_name;

    void setCIDRAddress(QHostAddress ipAddress, QHostAddress mask);

};

#endif