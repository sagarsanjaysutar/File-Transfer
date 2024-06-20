#include "ReceivingServer.h"
#include "networknamespace.h"

ReceivingServer::ReceivingServer(QObject* parent) :
    QObject(parent)
{
    qDebug() << "ReceivingServer: Constructor called.";
}

ReceivingServer::~ReceivingServer(){
    qDebug() << "ReceivingServer: Destructor called.";
    m_server.close();
}

void ReceivingServer::initServer(QString address){
    initServer(QHostAddress(address));
}

void ReceivingServer::initServer(QHostAddress address){
    qDebug() << "ReceivingServer: Initialising TCP Server.";
    if(!m_server.listen(address, 5444))
        qDebug() << "ReceivingServer: Unable to start TCP Server on" << m_interface.getIpAddress() << QString::number(Network::PORT);
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
    