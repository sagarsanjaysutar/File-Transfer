
#include "Network.h"

#include <QRegularExpression>
#include <QDebug>
#include <QProcess>

Network::Network(){
    qDebug() << "Network: Constructor called. ";
    m_localHostInterfaces = getLocalHostInterfaces();

    if(m_localHostInterfaces.count() == 0){
        qDebug() << "Network: No interfaces";
    }
    else{
        for(NetworkInterface interface : m_localHostInterfaces){
            QList<NetworkInterface> sftpInterfaces = getSFTPInterfaces(interface);

            // qDebug() << sftpInterfaces.at(0).m_ipAddress;
            QString resp = "Network: " + QString::number(sftpInterfaces.count()) + " SFTP interfaces found on " + interface.m_ipAddress.toString();
            qDebug() << resp;
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
                QRegularExpression rx("[0-9]{1,3}.[0-9]{1,3}.[0-9]{1,3}.[0-9]{1,3}");
                QRegularExpressionMatch ipMatch = rx.match(line);
                hosts << QHostAddress(ipMatch.captured(0));
            }
            else{
                // qDebug() << line << " closed.";
            }
        }
    }
    
    return hosts;
}

QList<NetworkInterface> Network::getSFTPInterfaces(NetworkInterface interface){

    QString nmapResp;
    QList<NetworkInterface> interfaces = QList<NetworkInterface>();
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
    QString localHostInterfaces;
    
    QString cmdStr = "nmap -p 22,2222 " + interface.m_CIDRAddress + " --exclude " + interface.m_ipAddress.toString();    
    cmd->start(cmdStr);
    qDebug() << "Network: Started " << cmdStr;

    if (!cmd->waitForStarted()){
        qDebug() << "Network: Failed to start the discovery process.";
        return interfaces;
    }

    if (!cmd->waitForFinished()){
        qDebug() << "Network: Discovery process timedout.";
        return interfaces;
    }

    qDebug() << "Network: Discovery execution completed";
    delete cmd;

    for(QHostAddress ipAddr : parseNmapResp(nmapResp)){
        interfaces.append(NetworkInterface(ipAddr, interface.m_maskAddress));
    }
    return interfaces;
}

Network::~Network(){
    qDebug() << "Network: Destructor called.";
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
                // qDebug() << "IP Address " << entry.ip().toString()
                // << "\tNetwork CIDR " << networkCIDRStr;
                networkInterfaces.append(NetworkInterface(entry.ip(), entry.netmask()));
            }
        }
    }

    return networkInterfaces;
}