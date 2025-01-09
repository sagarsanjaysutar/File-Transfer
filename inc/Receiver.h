#ifndef RECEIVER_H
#define RECEIVER_H

#include <QObject>
#include "DeviceInterface.h"
#include "SFTPAccess.h"

class Receiver : public QObject
{
    Q_OBJECT
public:
    Receiver(QObject *parent = nullptr);
    ~Receiver();

    Q_INVOKABLE void receiveFile(QString receiverAddress, QString fileName);
};

#endif
