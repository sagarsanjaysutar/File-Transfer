#include "Utils.h"

#include <QDebug>

QSharedPointer<Utils> Utils::m_instance = nullptr;

Utils::Utils(){
    qDebug() << "Utils: Constructor called.";
}

Utils::~Utils(){
    qDebug() << "Utils: Destructor called.";
}

QSharedPointer<Utils> Utils::getUtils(){

    if(m_instance != nullptr){
        qDebug() << "Utils: Creating new Utils object.";
        m_instance = QSharedPointer<Utils>(new Utils(), destroyInstance);        
    }
    else{
        qDebug() << "Utils: Returning existing Utils object.";
    }
    return m_instance;
}

void Utils::destroyInstance(Utils *obj){
    qDebug() << "Utils: Destroying instance.";
    // delete obj;
}

int Utils::startProcess(QString cmdStr){
    QProcess *cmd = new QProcess();
    QObject::connect(
        cmd,
        &QProcess::readyReadStandardOutput,
        [=](){
            qDebug() << "Command execution output received.";
            QString output(cmd->readAllStandardOutput());
            QStringList op = output.split("\n");
            foreach (QString line, op){
                qDebug() << line;
            }
        });
    cmd->start(cmdStr);

    if (!cmd->waitForStarted()){
        qDebug() << "Failed to start command execution";
        return -1;
    }

    if (!cmd->waitForFinished()){
        qDebug() << "Command execution timed out";
        return -1;
    }

    qDebug() << "Command execution completed";

    delete cmd;
    return 0;
}