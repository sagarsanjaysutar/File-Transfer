#include "DiscoveryThread.h"
#include "networknamespace.h"

DiscoveryThread::DiscoveryThread(QObject *parent) :
    QThread(parent)
{
    qDebug() << "DiscoveryThread: Constructor called.";
}

void DiscoveryThread::run(){

    qDebug() << "DiscoveryThread: Starting the discovery process...";
    
    QList<QSharedPointer<DeviceInterface>> localHostInterfaces = getLocalHostInterfaces();

    QSharedPointer<DeviceInterface> localHostInterface = localHostInterfaces.at(0);
    emit sig_detectedLocalHostInterface(localHostInterface);

    // List of empty interfaces.
    QList<QSharedPointer<DeviceInterface>> devicesOnNetwork;
    
    // nmap response
    QString nmapResp;

    // Initialise nmap process to search devices on localhost network.
    QProcess *nmapProcess = new QProcess();
    QObject::connect(nmapProcess, &QProcess::started, [=](){ qDebug() << "DiscoveryThread: Discovery execution started.";});
    QObject::connect(nmapProcess, &QProcess::finished, [=](int exitCode, QProcess::ExitStatus exitStatus){ qDebug() << "DiscoveryThread: Discovery execution finished." << exitCode << exitStatus;});
    QObject::connect(nmapProcess, &QProcess::readyReadStandardOutput, [&](){ nmapResp.append(nmapProcess->readAllStandardOutput()); });
    QObject::connect(nmapProcess, &QProcess::errorOccurred, [=](QProcess::ProcessError error){ qDebug() << "DiscoveryThread: Error Occured " << error; });
    QObject::connect(nmapProcess, &QProcess::stateChanged, [=](QProcess::ProcessState state){ qDebug() << "DiscoveryThread: State Changed " << state; });
    
    // List of localhost interfaces to exclude out as we won't be sharing files within same machine.
    QString excludedInterfacesStr;
    for(QSharedPointer<DeviceInterface> device : localHostInterfaces){
        excludedInterfacesStr.append(device.data()->getGatewayAddress().toString());
        excludedInterfacesStr.append(",");
        excludedInterfacesStr.append(device.data()->getIpAddress().toString());
        excludedInterfacesStr.append(",");
    }

    // nmap command.
    nmapProcess->setProgram("nmap");
    nmapProcess->setArguments(QStringList() 
            << "-p" << QString::number(Network::PORT)
            << localHostInterface.data()->getCIDRAddress()
            << "--exclude"
            << excludedInterfacesStr
            );
    nmapProcess->start();

    // Wait for the process to start
    if (!nmapProcess->waitForStarted()){
        qDebug() << "DiscoveryThread: Failed to start the discovery process.";
        emit sig_discoveredDevicesOnNetwork(devicesOnNetwork);
    }

    // Wait until the process is completed.
    if (!nmapProcess->waitForFinished()){
        qDebug() << "DiscoveryThread: Discovery process timedout.";
        emit sig_discoveredDevicesOnNetwork(devicesOnNetwork);
    }

    // Misc. cleanup
    delete nmapProcess;

    // Populate hostsOnInterface with the generated IP address from nmap process.
    // Note: All the IP address found by a nmap search would have same mask address.
    for(QHostAddress ipAddr : parseNmapResp(nmapResp)){
        devicesOnNetwork.append(QSharedPointer<DeviceInterface>(new DeviceInterface(ipAddr, localHostInterface.data()->getMaskAddress())));
    }

    qDebug() << "DiscoveryThread: Discovery execution completed. Found" << devicesOnNetwork.count() << " devices on" << localHostInterface.data()->getIpAddress().toString();
    emit sig_discoveredDevicesOnNetwork(devicesOnNetwork);
}

QList<QSharedPointer<DeviceInterface>> DiscoveryThread::getLocalHostInterfaces(){

    qDebug() << "DiscoveryThread: Getting localhost interfaces...";
    QList<QSharedPointer<DeviceInterface>> localHostInterfaces;

    // Iterate the network interfaces on the local system.
    for(const QNetworkInterface &interface : QNetworkInterface::allInterfaces()){
        for(QNetworkAddressEntry entry : interface.addressEntries()){
            if( interface.flags() & QNetworkInterface::IsRunning &&
                entry.ip().protocol() == QAbstractSocket::IPv4Protocol &&
                entry.ip() != QHostAddress(QHostAddress::LocalHost) &&
                !entry.ip().isLoopback())
            {
                qDebug() << "DiscoveryThread: Found" << interface.humanReadableName();
                localHostInterfaces.append(
                    QSharedPointer<DeviceInterface>(new DeviceInterface(
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
        qDebug() << "DiscoveryThread: Nmap response is empty.";
        return QList<QHostAddress>();
    }

    // Extract list of IP addresses from the nmap response.
    QList<QHostAddress> deviceAddressList = QList<QHostAddress>();

    // Parse the nmap response. Refer the nmap output to understand the following logic
    QStringList dataList = resp.split("\n");
    int idx = 0;
    while(idx < dataList.count()){
        QString line = dataList.at(idx);
        // qDebug() << line;
        if(line.contains("Nmap scan report")){
            QString portLine = dataList.at(idx + 4);
            
            if(portLine.contains(QString::number(Network::PORT)) ){ //&& portLine.contains("open")
                // Regex to fetch the IP address.
                QRegularExpression rx("[0-9]{1,3}.[0-9]{1,3}.[0-9]{1,3}.[0-9]{1,3}");
                QRegularExpressionMatch ipMatch = rx.match(line);
                deviceAddressList << QHostAddress(ipMatch.captured(0));
                // qDebug() << ipMatch.captured(0);
            }
        }

        idx++;
    }
    
    return deviceAddressList;
}