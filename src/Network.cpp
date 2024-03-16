#include "Network.h"
#include "Utils.h"

#include <QDebug>

Network::Network(){
    qDebug() << "Network: Constructor called.";
    QSharedPointer<Utils> utils = Utils::getUtils();
    utils.get()->startProcess("ls");
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
    

