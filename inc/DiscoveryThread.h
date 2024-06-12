#ifndef DISCOVERYTHREAD_H
#define DISCOVERYTHREAD_H

#include <QNetworkInterface>
#include <QHostAddress>
#include <QSharedPointer>
#include <QThread>
#include <QProcess>
#include <QRegularExpression>

#include "NetworkInterface.h"

/**
 * \brief Discovers the host interfaces on the localhost network. Similar to `nmap 192.168.0.0/24`
*/
class DiscoveryThread : public QThread{
    Q_OBJECT
public:
    DiscoveryThread(QObject *parent);
private:

    //!< @brief Runs the nmap process for discovering hosts.
    void run() override;

    //!< @brief Returns a list of local system's network interfaces like Wifi, ethernet, etc.
    QList<QSharedPointer<NetworkInterface>> getLocalHostInterfaces();

    //!< @brief Helper function to parse the nmap response & return a list of hosts IP address.
    QList<QHostAddress> parseNmapResp(QString resp);

signals: 
    void sig_discoveredHosts(QList<QSharedPointer<NetworkInterface>> hosts);
};

#endif