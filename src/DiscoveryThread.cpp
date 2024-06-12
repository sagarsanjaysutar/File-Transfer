#include "DiscoveryThread.h"
#include "networknamespace.h"

DiscoveryThread::DiscoveryThread(QObject *parent) :
QThread(parent)
{
    qDebug() << "DiscoveryThread: Constructor called.";
}

void DiscoveryThread::run(){

    qDebug() << "DiscoveryThread: Starting the discovery process...";
    
    // List of all localhost interfaces.
    QList<QSharedPointer<NetworkInterface>> localHostInterfaces = getLocalHostInterfaces(); 
    
    // Search hosts on the first interface.
    QSharedPointer<NetworkInterface> interface = localHostInterfaces.at(0);

    // List of empty interfaces.
    QList<QSharedPointer<NetworkInterface>> hostsOnInterface;
    
    // nmap response
    QString nmapResp;

    // Initialise nmap process to search hosts.
    QProcess *nmapProcess = new QProcess();
    QObject::connect(nmapProcess, &QProcess::started, [=](){ qDebug() << "NetworkManager: Discovery execution started.";});
    QObject::connect(nmapProcess, &QProcess::finished, [=](int exitCode, QProcess::ExitStatus exitStatus){ qDebug() << "NetworkManager: Discovery execution finished." << exitCode << exitStatus;});
    QObject::connect(nmapProcess, &QProcess::readyReadStandardOutput, [&](){ nmapResp.append(nmapProcess->readAllStandardOutput()); });
    QObject::connect(nmapProcess, &QProcess::errorOccurred, [=](QProcess::ProcessError error){ qDebug() << "Error Occured " << error; });
    QObject::connect(nmapProcess, &QProcess::stateChanged, [=](QProcess::ProcessState state){ qDebug() << "State Changed " << state; });
    
    // Make a list of localhost to exclude out of the search as we won't be sharing files within same computer.
    QString excludedInterfacesStr;
    for(QSharedPointer<NetworkInterface> interface : localHostInterfaces){
        excludedInterfacesStr.append(interface.data()->getGatewayAddress().toString());
        excludedInterfacesStr.append(",");
        excludedInterfacesStr.append(interface.data()->getIpAddress().toString());
        excludedInterfacesStr.append(",");
    }

    // nmap command.
    nmapProcess->setProgram("nmap");
    nmapProcess->setArguments(QStringList() 
            << "-p" << QString::number(Network::PORT)
            << interface.data()->getCIDRAddress()
            << "--exclude"
            << excludedInterfacesStr
            );
    nmapProcess->start();

    // Wait for the process to start
    if (!nmapProcess->waitForStarted()){
        qDebug() << "NetworkManager: Failed to start the discovery process.";
        emit sig_discoveredHosts(hostsOnInterface);
    }

    // Wait until the process is completed.
    if (!nmapProcess->waitForFinished()){
        qDebug() << "NetworkManager: Discovery process timedout.";
        emit sig_discoveredHosts(hostsOnInterface);
    }

    // Misc. cleanup
    delete nmapProcess;

    // Populate hostsOnInterface with the generated IP address from nmap process.
    // Note: All the IP address found by a nmap search would have same mask address.
    for(QHostAddress ipAddr : parseNmapResp(nmapResp)){
        hostsOnInterface.append(QSharedPointer<NetworkInterface>(new NetworkInterface(ipAddr, interface.data()->getMaskAddress())));
    }

    qDebug() << "NetworkManager: Discovery execution completed. Found" << hostsOnInterface.count() << "hosts on" << interface.data()->getIpAddress().toString();
    emit sig_discoveredHosts(hostsOnInterface);
}

QList<QSharedPointer<NetworkInterface>> DiscoveryThread::getLocalHostInterfaces(){

    qDebug() << "DiscoveryThread: Getting localhost interfaces.";
    QList<QSharedPointer<NetworkInterface>> localHostInterfaces;

    // Iterate the network interfaces on the local system.
    for(const QNetworkInterface &interface : QNetworkInterface::allInterfaces()){
        for(QNetworkAddressEntry entry : interface.addressEntries()){
            if( interface.flags() & QNetworkInterface::IsRunning &&
                entry.ip().protocol() == QAbstractSocket::IPv4Protocol &&
                entry.ip() != QHostAddress(QHostAddress::LocalHost) &&
                !entry.ip().isLoopback())
            {
                qDebug() << "NetworkManager: Found" << interface.humanReadableName();
                localHostInterfaces.append(
                    QSharedPointer<NetworkInterface>(new NetworkInterface(
                        entry.ip(),
                        entry.netmask(), 
                        interface.humanReadableName(), 
                        interface.type())
                    )
                );
            }
        }
    }

    return localHostInterfaces;
}

QList<QHostAddress> DiscoveryThread::parseNmapResp(QString resp){

    if(resp.isEmpty()){
        qDebug() << "NetworkManager: Nmap response is empty.";
        return QList<QHostAddress>();
    }

    // Extract list of IP addresses from the nmap response.
    QList<QHostAddress> hosts = QList<QHostAddress>();

    // Parse the nmap response. Refer the nmap output to understand the following logic
    QStringList dataList = resp.split("\n");
    int idx = 0;
    while(idx < dataList.count()){
        QString line = dataList.at(idx);
        qDebug() << line;
        if(line.contains("Nmap scan report")){
            QString portLine = dataList.at(idx + 4);
            
            if(portLine.contains(QString::number(Network::PORT)) ){ //&& portLine.contains("open")
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