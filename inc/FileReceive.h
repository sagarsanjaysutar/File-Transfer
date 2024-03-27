#ifndef FILERECEIVE_H
#define FILERECEIVE_H

#include <QTcpServer>
#include <QTcpSocket>
#include "NetworkInterface.h"

class FileReceive{
public:
    FileReceive(NetworkInterface interface);
    ~FileReceive();
    

private:  
    NetworkInterface m_interface;
    QTcpServer m_server;
    QTcpSocket* m_clientConnection;

    const quint16 PORT = 5444;

    void initServer();
    void receiveFile();
};

#endif

