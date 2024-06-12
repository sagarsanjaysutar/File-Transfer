#include "NetworkManager.h"
#include "networknamespace.h"

NetworkManager::NetworkManager(QObject *parent) :   
    QObject(parent)
 {
    qDebug() << "NetworkManager: Constructor called.";
}

NetworkManager::~NetworkManager(){
    qDebug() << "NetworkManager: Destructor called.";
}

void NetworkManager::initHostInterfaces(){
    qDebug() << "NetworkManager: Discovering & settings host interfaces.";
    m_discoveryThread = new DiscoveryThread(this);
    connect(m_discoveryThread, &DiscoveryThread::sig_discoveredHosts, this, [&](QList<QSharedPointer<NetworkInterface>> interfaces){
        qDebug() << "Discovered " << interfaces.count() << " interfaces.";
        for(QSharedPointer<NetworkInterface> interface : interfaces){
            m_hostInterfaces.append(QVariant::fromValue(interface.data()));
            emit sig_hostsUpdated();
        }
    });

    m_discoveryThread->start();
}