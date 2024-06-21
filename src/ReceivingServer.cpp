#include "ReceivingServer.h"
#include "networknamespace.h"

ReceivingServer::ReceivingServer(QSharedPointer<DeviceInterface> localHostInteface, QObject* parent) :
    QObject(parent),
    m_server(parent),
    m_localHostInteface(localHostInteface)
{
    qDebug() << "ReceivingServer: Constructor called.";
}

ReceivingServer::~ReceivingServer(){
    qDebug() << "ReceivingServer: Destructor called.";
    m_server.close();
}

void ReceivingServer::initServer(){
    if(m_localHostInteface == nullptr){
        qDebug() << "ReceivingServer: Invalid localhost. Can't start TCP Server.";
        return;
    }
    else if(!m_server.listen(m_localHostInteface.data()->getIpAddress(), 5000)){
        qDebug() << "ReceivingServer: Unable to start TCP Server on" << m_localHostInteface.data()->getIpAddress() << QString::number(Network::PORT);
        return;
    }
    else
        qDebug() << "ReceivingServer: Started TCP Server on" << m_server.serverAddress() << m_server.serverPort();

    QObject::connect(&m_server, &QTcpServer::newConnection, [&](){
        qDebug() << "ReceivingServer: New connection";
        m_clientConnection = m_server.nextPendingConnection();
        QObject::connect(m_clientConnection, &QIODevice::readyRead, [&](){ receiveFile(); });    
    });

    qDebug() << "ReceivingServer: Waiting for new connection...";
    m_server.waitForNewConnection(30000);
}

void ReceivingServer::receiveFile(){
    // initServer();
    // in.startTransaction();

    // QString nextFortune;
    // in >> nextFortune;

    // if (!in.commitTransaction())
    //     return;

    // if (nextFortune == currentFortune) {
    //     QTimer::singleShot(0, this, &Client::requestNewFortune);
    //     return;
    // }

    // currentFortune = nextFortune;
    // statusLabel->setText(currentFortune);
    // getFortuneButton->setEnabled(true);
}
    