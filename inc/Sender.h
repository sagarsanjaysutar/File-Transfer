#ifndef SENDER_H
#define SENDER_H

#include <QObject>
#include "DeviceInterface.h"
#include "SFTPAccess.h"

class Sender : public QObject
{
    Q_OBJECT
public:
    Sender(QObject *parent = nullptr);
    ~Sender();

    Q_INVOKABLE void sendFile(QString receiverAddress, QString fileName);
};

#endif
