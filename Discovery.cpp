#include <QCoreApplication>
#include <QProcess>
#include <QDebug>
#include <fstream>
#include <QNetworkInterface>

QString getCIDRNotation(QHostAddress ipAddress, QHostAddress subnetMask){
    
    // Convert the IP Address into integer.
    quint32 subnetAddrInt = subnetMask.toIPv4Address();
    
    // Initialize the CIDR notation
    int cidr = 0;
    
    // Count the number of "1" in the subnet mask
    while(subnetAddrInt){
        cidr = cidr + (subnetAddrInt & 1);
        subnetAddrInt = subnetAddrInt >> 1;
    }

    QStringList ipAddrList = ipAddress.toString().split(".");
    ipAddrList[ipAddrList.count() - 1] = "0";
    return ipAddrList.join(".").append("/").append(QString::number(cidr));
}

int main(int argc, char *argv[]){
    QCoreApplication a(argc, argv);
    qDebug() << getCIDRNotation(QHostAddress(QString("192.168.0.172")), QHostAddress(QString("255.255.255.0")));
    return 0;
    QProcess *cmd = new QProcess();
    QObject::connect(
        cmd,
        &QProcess::readyReadStandardOutput,
        [=](){
             qDebug() << "Finished";
            QString output(cmd->readAllStandardOutput());
            QStringList op = output.split("\n");
            foreach(QString line , op){
                qDebug() << "\t " << line;
            }
        });

    // struct NetworkInterface{
    //     QHostAddress ip;
    //     QHostAddress subnetMask;
    //     qint8 CIDRNotation;

    // }
    
        
    // const QHostAddress &localhost = QHostAddress(QHostAddress::LocalHost);
    // const QHostAddress &localhost = ;
    for(const QNetworkInterface &interface : QNetworkInterface::allInterfaces()){
        for(QNetworkAddressEntry entry : interface.addressEntries()){
            if( interface.flags() & QNetworkInterface::IsRunning;
                entry.ip().protocol() == QAbstractSocket::IPv4Protocol &&
                entry.ip() != QHostAddress(QHostAddress::LocalHost) &&
                !entry.ip().isLoopback()
                ){
                qDebug() << " ==== " << entry.ip() << "\t" << entry.netmask() << entry.netmask().toIPv4Address();
            }
        }
    }
    // cmd->start("ifconfig");

    // if (!cmd->waitForStarted()) {
    //     qDebug() << "Failed to start command execution";
    //     return -1;
    // }

    // if (!cmd->waitForFinished()) {
    //     qDebug() << "Command execution timed out";
    //     return -1;
    // }

    qDebug() << "Command execution completed";

    delete cmd;

    return a.exec();
}