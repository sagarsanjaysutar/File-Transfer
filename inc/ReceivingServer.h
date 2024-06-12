#ifndef RECEIVINGSERVER_H
#define RECEIVINGSERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QObject>
#include "NetworkInterface.h"

class ReceivingServer : public QObject{
    Q_OBJECT
public:
    ReceivingServer(NetworkInterface interface, QObject* parent = nullptr);
    ~ReceivingServer();

    Q_INVOKABLE void receiveFile();

private:  
    NetworkInterface m_interface;
    QTcpServer m_server;
    QTcpSocket* m_clientConnection;

    void initServer();
};

#endif

