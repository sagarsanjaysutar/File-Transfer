#include "Receiver.h"
#include "networknamespace.h"

Receiver::Receiver(QObject *parent) : QObject(parent)
{
    qDebug() << "Receiver: Constructor called.";
}

Receiver::~Receiver()
{
    qDebug() << "Receiver: Destructor called.";
}

void Receiver::receiveFiles(QString receiverAddress, QString fileName)
{
    // QString cmd("sftp root-temp@s-5a57405a863a4ce79.server.transfer.ca-central-1.amazonaws.com <<< $'");

}