#ifndef RECEIVINGSERVER_H
#define RECEIVINGSERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QObject>
#include "DeviceInterface.h"

class ReceivingServer : public QObject{
    Q_OBJECT
public:
    ReceivingServer(QObject* parent = nullptr);
    ~ReceivingServer();

    Q_INVOKABLE void receiveFile();

    //!< \brief A string-overloaded function for initServer(...).
    Q_INVOKABLE void initServer(QString address);

private:
    DeviceInterface m_interface;
    QTcpServer m_server;
    QTcpSocket* m_clientConnection;

    //!< \brief Starts the TCP server on the given IP address to listen for incoming connections.
    void initServer(QHostAddress address);
};

#endif

