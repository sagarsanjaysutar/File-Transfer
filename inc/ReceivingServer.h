#ifndef RECEIVINGSERVER_H
#define RECEIVINGSERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include "NetworkInterface.h"

class ReceivingServer{
public:
    ReceivingServer(NetworkInterface interface);
    ~ReceivingServer();

private:  
    NetworkInterface m_interface;
    QTcpServer m_server;
    QTcpSocket* m_clientConnection;


    void initServer();
    void receiveFile();
};

#endif

