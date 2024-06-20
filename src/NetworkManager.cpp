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

void NetworkManager::startDeviceDiscovery(){
    qDebug() << "NetworkManager: Starting the device discovery process...";

    m_discoveryThread = new DiscoveryThread(this);

    connect(m_discoveryThread, &DiscoveryThread::sig_detectedLocalHostInterface, this, [&](QSharedPointer<DeviceInterface> interface){
        qDebug() << "NetworkManager: Discovered localhost inteface: " << interface;
        m_localHostInterface = interface;
    });

    connect(m_discoveryThread, &DiscoveryThread::sig_discoveredDevicesOnNetwork, this, [&](QList<QSharedPointer<DeviceInterface>> devices){
        qDebug() << "NetworkManager: Discovered " << devices.count() << " devices on localhost network.";
        m_devicesOnNetwork = devices;
        emit sig_devicesOnNetworkUpdated();
    });

    m_discoveryThread->start();
}

QVariantList NetworkManager::getDevicesOnNetwork(){
    QVariantList devicesOnNetwork;
    for(QSharedPointer<DeviceInterface> device : m_devicesOnNetwork){
        devicesOnNetwork.append(QVariant::fromValue(device.data()));
    }
    return devicesOnNetwork;
}