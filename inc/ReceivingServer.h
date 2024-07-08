#ifndef RECEIVINGSERVER_H
#define RECEIVINGSERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QObject>
#include "DeviceInterface.h"

class ReceivingServer : public QObject{
    Q_OBJECT
public:
    ReceivingServer(QSharedPointer<DeviceInterface> localHostInteface, QObject* parent = nullptr);
    ~ReceivingServer();

    Q_INVOKABLE void receiveFile();

    //!< \brief Starts a TCP server on the localhost network to listen for incoming connections.
    Q_INVOKABLE void initServer();

    //!< \brief Stops the TCP server started on the localhost network.
    Q_INVOKABLE void stopServer();

private:
    QTcpServer m_server;
    QTcpSocket* m_clientConnection;
    QSharedPointer<DeviceInterface> m_localHostInteface;
};

#endif

