#include "Sender.h"
#include "networknamespace.h"

Sender::Sender(QObject *parent) : QObject(parent)
{
    qDebug() << "Sender: Constructor called.";
}

Sender::~Sender()
{
    qDebug() << "Sender: Destructor called.";
}

void Sender::sendFile(QString receiverAddress, QString fileName)
{
    // QString cmd("sftp root-temp@s-5a57405a863a4ce79.server.transfer.ca-central-1.amazonaws.com <<< $'");
}