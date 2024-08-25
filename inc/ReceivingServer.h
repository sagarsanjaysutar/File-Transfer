#ifndef RECEIVINGSERVER_H
#define RECEIVINGSERVER_H

#include <QObject>
#include "DeviceInterface.h"
#include <SFTPAccess.h>

class ReceivingServer : public QObject
{
    Q_OBJECT
public:
    ReceivingServer(QSharedPointer<SFTPAccess> sftp, QObject *parent = nullptr);
    ~ReceivingServer();

    Q_INVOKABLE void receiveFile();

    //!< \brief Starts a TCP server on the localhost network to listen for incoming connections.
    Q_INVOKABLE void initServer();

    //!< \brief Stops the TCP server started on the localhost network.
    Q_INVOKABLE void stopServer();

private:
    QSharedPointer<SFTPAccess> m_sftp;
};

#endif
