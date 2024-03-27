#include "FileReceive.h"

FileReceive::FileReceive(NetworkInterface interface) :
    m_interface(interface)
{
    qDebug() << "FileReceive: Constructor called.";
    initServer();
}

FileReceive::~FileReceive(){
    qDebug() << "FileReceive: Destructor called.";
}

void FileReceive::initServer(){
    qDebug() << "FileReceive: Initialising TCP Server.";
    if(!m_server.listen(m_interface.getIpAddress(), PORT))
        qDebug() << "FileReceive: Unable to start TCP Server on" << m_interface.getIpAddress() << PORT;
    else
        qDebug() << "FileReceive: Start TCP Server on" << m_server.serverAddress() << m_server.serverPort();

    QObject::connect(&m_server, &QTcpServer::newConnection, [&](){ 
        m_clientConnection = m_server.nextPendingConnection();
        QObject::connect(m_clientConnection, &QIODevice::readyRead, [&](){ receiveFile(); });    
    });
    
}

void FileReceive::receiveFile(){
    qDebug() << "Receiving file...";
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
    