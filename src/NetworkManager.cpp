#include "NetworkManager.h"
#include "networknamespace.h"

NetworkManager::NetworkManager(QObject *parent) :   
    QObject(parent),
    m_discoveryThread(nullptr),
    m_deviceDiscoveryWorker(nullptr),
    m_devicesOnNetwork()
{
    qDebug() << "NetworkManager: Constructor called.";
}

NetworkManager::~NetworkManager(){
    qDebug() << "NetworkManager: Destructor called.";
}

QString NetworkManager::getLocalHostIPAddress(){
    return getLocalHostInterface().data()->getIpAddress().toString();
}

QSharedPointer<DeviceInterface> NetworkManager::getLocalHostInterface(){
    return getLocalHostInterfaces().at(0);
}

void NetworkManager::startDeviceDiscovery(){
    
    if(m_discoveryThread != nullptr && m_discoveryThread->isRunning()){
        qDebug() << "NetworkManager: Can't start device discovery process, as it is already running.";
        return;
    }

    qDebug() << "NetworkManager: Starting the device discovery process...";

    m_discoveryThread = new QThread();
    m_deviceDiscoveryWorker = new DeviceDiscovery(getLocalHostInterface());
    m_deviceDiscoveryWorker->moveToThread(m_discoveryThread);

    // Start the device discovery process when the thread starts.
    connect(m_discoveryThread, &QThread::started, m_deviceDiscoveryWorker, &DeviceDiscovery::slot_startDeviceDiscovery);

    // Handle the discovery response.
    connect(m_deviceDiscoveryWorker, &DeviceDiscovery::sig_discoveredDevicesOnNetwork, this, [&](QList<QSharedPointer<DeviceInterface>> devices){
        qDebug() << "NetworkManager: Discovered " << devices.count() << " devices on localhost network.";
        m_devicesOnNetwork = devices;
        emit sig_devicesOnNetworkUpdated();
        m_discoveryThread->quit();
    });

    // Clean up
    connect(m_discoveryThread, &QThread::finished, m_discoveryThread, &QObject::deleteLater);
    connect(m_discoveryThread, &QThread::finished, m_deviceDiscoveryWorker, &QObject::deleteLater);

    // Start the discovery thread.
    m_discoveryThread->start();
}

QVariantList NetworkManager::getDevicesOnNetwork(){
    QVariantList devicesOnNetwork;
    for(QSharedPointer<DeviceInterface> device : m_devicesOnNetwork){
        devicesOnNetwork.append(QVariant::fromValue(device.data()));
    }
    return devicesOnNetwork;
}

QList<QSharedPointer<DeviceInterface>> NetworkManager::getLocalHostInterfaces(){

    qDebug() << "NetworkManager: Getting localhost interfaces...";
    QList<QSharedPointer<DeviceInterface>> localHostInterfaces;

    // Iterate the network interfaces on the local system.
    for(const QNetworkInterface &interface : QNetworkInterface::allInterfaces()){
        for(QNetworkAddressEntry entry : interface.addressEntries()){
            if( interface.flags() & QNetworkInterface::IsRunning &&
                entry.ip().protocol() == QAbstractSocket::IPv4Protocol &&
                entry.ip() != QHostAddress(QHostAddress::LocalHost) &&
                !entry.ip().isLoopback())
            {
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