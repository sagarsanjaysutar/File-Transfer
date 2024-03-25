
#include "Network.h"
#include <QRegularExpression>
#include <QDebug>
#include <QProcess>

Network::Network(){
    qDebug() << "Network: Constructor called.";

    // Discover localhost interfaces
    m_localHostInterfaces = getLocalHostInterfaces();


    if(m_localHostInterfaces.count() == 0){
        qDebug() << "Network: No interfaces";
    }
    else{

        // Iterate localhost interfaces to discover SFTP hosts within each of those interface's internal network.
        for(NetworkInterface interface : m_localHostInterfaces){
            QList<NetworkInterface> sftpInterfaces = getSFTPInterfaces(interface);

            QString resp = "Network: " + QString::number(sftpInterfaces.count()) 
            + " SFTP interfaces found on " + interface.getIpAddress().toString();
            qDebug() << resp;
        }
    }
}

Network::~Network(){
    qDebug() << "Network: Destructor called.";
}

QList<NetworkInterface> Network::getLocalHostInterfaces(){
    QList<NetworkInterface> networkInterfaces;
    
    // Build local host interfaces.
    for(const QNetworkInterface &interface : QNetworkInterface::allInterfaces()){
        for(QNetworkAddressEntry entry : interface.addressEntries()){
            if( interface.flags() & QNetworkInterface::IsRunning &&
                entry.ip().protocol() == QAbstractSocket::IPv4Protocol &&
                entry.ip() != QHostAddress(QHostAddress::LocalHost) &&
                !entry.ip().isLoopback()
                ){
                networkInterfaces.append(NetworkInterface(entry.ip(), entry.netmask()));
            }
        }
    }

    return networkInterfaces;
}

QList<NetworkInterface> Network::getSFTPInterfaces(NetworkInterface interface){

    // nmap response
    QString nmapResp;

    // List of empty interfaces.
    QList<NetworkInterface> interfaces = QList<NetworkInterface>();

    // Initialise nmap process
    QProcess *nmapProcess = new QProcess();
    QObject::connect(nmapProcess, &QProcess::readyReadStandardOutput, [&](){ nmapResp.append(nmapProcess->readAllStandardOutput()); });

    // Make a list of localhost to exclude out of the search as we won't be sharing files within same computer.
    QString localHostInterfaces;
    for(NetworkInterface interface : m_localHostInterfaces){
        localHostInterfaces.append(interface.getIpAddress().toString());
        localHostInterfaces.append(",");
    }

    // nmap command.
    QString cmdStr = "nmap -p 22,2222 " + interface.getCIDRAddress() + " --exclude " + localHostInterfaces;    
    nmapProcess->start(cmdStr);
    qDebug() << "Network: Started " << cmdStr;

    // Wait for the process to start
    if (!nmapProcess->waitForStarted()){
        qDebug() << "Network: Failed to start the discovery process.";
        return interfaces;
    }

    // Wait until the process is completed.
    if (!nmapProcess->waitForFinished()){
        qDebug() << "Network: Discovery process timedout.";
        return interfaces;
    }

    // Misc. cleanup
    qDebug() << "Network: Discovery execution completed";
    delete nmapProcess;

    // Once the nmap process is completed, & list of IP address are found, make a list NetworkInterfaces.
    // Note: All the IP address found by the above nmap search would have same mask address.
    for(QHostAddress ipAddr : parseNmapResp(nmapResp)){
        interfaces.append(NetworkInterface(ipAddr, interface.getMaskAddress()));
    }
    return interfaces;
}

QList<QHostAddress> Network::parseNmapResp(QString resp){

    if(resp.isEmpty()){
        return QList<QHostAddress>();
    }

    // Extract list of IP addresses from the nmap response.
    QList<QHostAddress> hosts = QList<QHostAddress>();

    // Parse the nmap response. Refer the nmap output to understand the following logic
    QStringList dataList = resp.split("\n");
    for(int idx = 0; idx < dataList.count(); idx++){
        QString line = dataList.at(idx);
        if(line.contains("Nmap scan report")){
            if(dataList.at(idx + 4).contains("open") || dataList.at(idx + 5).contains("open")){
                // Regex to fetch the IP address.
                QRegularExpression rx("[0-9]{1,3}.[0-9]{1,3}.[0-9]{1,3}.[0-9]{1,3}");
                QRegularExpressionMatch ipMatch = rx.match(line);
                hosts << QHostAddress(ipMatch.captured(0));
            }
        }
    }
    
    return hosts;
}