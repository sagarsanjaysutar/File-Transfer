#include "NetworkManager.h"
#include <QRegularExpression>
#include <QDebug>
#include <QProcess>

NetworkManager::NetworkManager(){
    qDebug() << "NetworkManager: Constructor called.";

    // Discover interfaces on the local system.
    setLocalHostInterfaces();

    // Discover host interfaces on each of the local interfaces.
    // setHostInterfaces();
}

NetworkManager::~NetworkManager(){
    qDebug() << "NetworkManager: Destructor called.";
}

void NetworkManager::setLocalHostInterfaces(){
    
    // Iterate the network interfaces on the local system.
    for(const QNetworkInterface &interface : QNetworkInterface::allInterfaces()){
        for(QNetworkAddressEntry entry : interface.addressEntries()){
            if( interface.flags() & QNetworkInterface::IsRunning &&
                entry.ip().protocol() == QAbstractSocket::IPv4Protocol &&
                entry.ip() != QHostAddress(QHostAddress::LocalHost) &&
                !entry.ip().isLoopback()
                ){
                m_localHostInterfaces.append(NetworkInterface(entry.ip(), entry.netmask()));
            }
        }
    }
}

void NetworkManager::setHostInterfaces(){

    if(m_localHostInterfaces.count() == 0){
        qDebug() << "NetworkManager: No local interfaces found. Can't discover hosts.";
        return;
    }
    else{
        qDebug() << "NetworkManager:" << m_localHostInterfaces.count() << "local interfaces found. Discovering hosts on it.";

        // Iterate interfaces on the local system to discover hosts.
        for(NetworkInterface interface : m_localHostInterfaces){
            m_hostInterfaces.insert(interface, discoverHostInterfaces(interface));
        }
    }
}

QList<NetworkInterface> NetworkManager::discoverHostInterfaces(NetworkInterface interface){

    // nmap response
    QString nmapResp;

    // List of empty interfaces.
    QList<NetworkInterface> hostInterfaces = QList<NetworkInterface>();

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
    QString cmdStr = "nmap -p 80 " + interface.getCIDRAddress() + " --exclude " + localHostInterfaces;    
    nmapProcess->start(cmdStr);
    qDebug() << "NetworkManager: Discovery execution started. " << cmdStr;

    // Wait for the process to start
    if (!nmapProcess->waitForStarted()){
        qDebug() << "NetworkManager: Failed to start the discovery process.";
        return hostInterfaces;
    }

    // Wait until the process is completed.
    if (!nmapProcess->waitForFinished()){
        qDebug() << "NetworkManager: Discovery process timedout.";
        return hostInterfaces;
    }

    // Misc. cleanup
    delete nmapProcess;

    // Populate QList<NetworkInterface> with the generated IP address from nmap process.
    // Note: All the IP address found by a nmap search would have same mask address.
    for(QHostAddress ipAddr : parseNmapResp(nmapResp)){
        hostInterfaces.append(NetworkInterface(ipAddr, interface.getMaskAddress()));
    }

    qDebug() << "NetworkManager: Discovery execution completed. Found" << hostInterfaces.count() << "hosts on" << interface.getIpAddress().toString();

    return hostInterfaces;
}

QList<QHostAddress> NetworkManager::parseNmapResp(QString resp){

    if(resp.isEmpty()){

        return QList<QHostAddress>();
    }

    // Extract list of IP addresses from the nmap response.
    QList<QHostAddress> hosts = QList<QHostAddress>();

    // Parse the nmap response. Refer the nmap output to understand the following logic
    QStringList dataList = resp.split("\n");
    int idx = 0;
    while(idx < dataList.count()){
        QString line = dataList.at(idx);
        if(line.contains("Nmap scan report")){
            QString portLine = dataList.at(idx + 4);

            if(portLine.contains("80") && portLine.contains("open")){
                // Regex to fetch the IP address.
                QRegularExpression rx("[0-9]{1,3}.[0-9]{1,3}.[0-9]{1,3}.[0-9]{1,3}");
                QRegularExpressionMatch ipMatch = rx.match(line);
                hosts << QHostAddress(ipMatch.captured(0));
                // qDebug() << ipMatch.captured(0);
            }
        }

        idx++;
    }
    
    return hosts;
}