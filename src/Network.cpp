#include "Network.h"
#include <QRegExp>
#include <QDebug>
#include <QProcess>

Network::Network(){
    qDebug() << "Network: Constructor called. " << getCIDRAddress(QHostAddress("192.168.92.213"), QHostAddress("255.255.255.0"));
    m_localHostInterfaces = getLocalHostInterfaces();

    if(m_localHostInterfaces.count() == 0){
        qDebug() << "Network: No interfaces";
    }
    else if(m_localHostInterfaces.count() == 1){
        QString resp = "Network: 1 interface found " + m_localHostInterfaces.at(0).m_ip.toString() + " with " +
        QString::number(getSFTPHosts(m_localHostInterfaces.at(0)).count()) + " active SFTP hosts.";
        qDebug() << resp;
    }
    else{
        qDebug() << "Network: Multiple interfaces found.";
        for(NetworkInterface interface : m_localHostInterfaces){
            getSFTPHosts(interface);
        }
    }
}

QList<QHostAddress> Network::parseNmapResp(QString resp){
    if(resp.isEmpty()){
        return QList<QHostAddress>();
    }
    QList<QHostAddress> hosts = QList<QHostAddress>();
    QStringList dataList = resp.split("\n");
    for(int idx = 0; idx < dataList.count(); idx++){
        QString line = dataList.at(idx);
        if(line.contains("Nmap scan report")){
            if(dataList.at(idx + 4).contains("open") || dataList.at(idx + 5).contains("open")){
                QRegExp rx("[0-9]{1,3}.[0-9]{1,3}.[0-9]{1,3}.[0-9]{1,3}");
                rx.indexIn(line);
                hosts << QHostAddress(rx.cap(0));
            }
            else{
                // qDebug() << line << " closed.";
            }
        }
    }
    
    return hosts;
}

QList<QHostAddress> Network::getSFTPHosts(NetworkInterface interface){

    QString nmapResp;
    QList<QHostAddress> hosts = QList<QHostAddress>();
    QProcess *cmd = new QProcess();
    QObject::connect(
        cmd,
        &QProcess::readyReadStandardOutput,
        [&](){
            // qDebug() << "Network: Discovered hosts.";
            nmapResp.append(cmd->readAllStandardOutput());
            // qDebug() << data;
            //             
        });
    QString cmdStr = "nmap -p 22,2222 " + interface.m_networkCIDR + " --exclude " + interface.m_ip.toString();    
    cmd->start(cmdStr);
    qDebug() << "Network: Started " << cmdStr;

    if (!cmd->waitForStarted()){
        qDebug() << "Network: Failed to start the discovery process.";
        return hosts;
    }

    if (!cmd->waitForFinished()){
        qDebug() << "Network: Discovery process timedout.";
        return hosts;
    }

    qDebug() << "Network: Discovery execution completed";
    delete cmd;

    return parseNmapResp(nmapResp);;
}

Network::~Network(){
    qDebug() << "Network: Destructor called.";
}

QString Network::getCIDRAddress(QHostAddress ipAddress, QHostAddress subnetMask){
    
    // Convert the IP Address into integer.
    quint32 subnetAddrInt = subnetMask.toIPv4Address();
    
    // Initialize the CIDR notation
    int cidr = 0;
    
    // Count the number of "1" in the subnet mask
    while(subnetAddrInt){
        cidr = cidr + (subnetAddrInt & 1);
        subnetAddrInt = subnetAddrInt >> 1;
    }

    QStringList ipAddrList = ipAddress.toString().split(".");
    ipAddrList[ipAddrList.count() - 1] = "0";
    return ipAddrList.join(".").append("/").append(QString::number(cidr));
}

QList<NetworkInterface> Network::getLocalHostInterfaces(){
    QList<NetworkInterface> networkInterfaces;
    
    // Iterate through networks.
    for(const QNetworkInterface &interface : QNetworkInterface::allInterfaces()){
        for(QNetworkAddressEntry entry : interface.addressEntries()){
            if( interface.flags() & QNetworkInterface::IsRunning &&
                entry.ip().protocol() == QAbstractSocket::IPv4Protocol &&
                entry.ip() != QHostAddress(QHostAddress::LocalHost) &&
                !entry.ip().isLoopback()
                ){
                QString networkCIDRStr = getCIDRAddress(entry.ip(), entry.netmask());
                // qDebug() << "IP Address " << entry.ip().toString()
                // << "\tNetwork CIDR " << networkCIDRStr;
                networkInterfaces.append(NetworkInterface(networkCIDRStr, interface));
            }
        }
    }

    return networkInterfaces;
}