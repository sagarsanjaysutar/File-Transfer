#include "SFTPAccess.h"
#include <QDebug>

SFTPAccess::SFTPAccess() {
    qDebug() << "SFTPAccess: Constructor called.";
    enableSFTP();
}

SFTPAccess::~SFTPAccess(){
    qDebug() << "SFTPAccess: Destructor called.";
}

bool SFTPAccess::sftpAvailableCheck(){

    qDebug() << "SFTPAccess: Checking if SFTP is running...";

    // Interpret SFTP check response.
    QString sftpAvailableCheckResp;
    QProcess *sftpAvailableCheckProcess = new QProcess();
    QObject::connect(sftpAvailableCheckProcess, &QProcess::finished, [=](int exitCode, QProcess::ExitStatus exitStatus){ qDebug() << "SFTPAccess: SFTP check finished." << exitCode << exitStatus;});
    QObject::connect(sftpAvailableCheckProcess, &QProcess::readyReadStandardOutput, [&](){ sftpAvailableCheckResp.append(sftpAvailableCheckProcess->readAllStandardOutput()); });
    QObject::connect(sftpAvailableCheckProcess, &QProcess::errorOccurred, [=](QProcess::ProcessError error){ qDebug() << "SFTPAccess: SFTP check Error Occurred. " << error; });
    
    sftpAvailableCheckProcess->setProgram("service");
    sftpAvailableCheckProcess->setArguments(QStringList() << "ssh" << "status");
    sftpAvailableCheckProcess->start();

    // Wait for the process to start
    if (!sftpAvailableCheckProcess->waitForStarted())
        qDebug() << "SFTPAccess: Failed to start the SFTP check process.";

    // Wait until the process is completed.
    if (!sftpAvailableCheckProcess->waitForFinished())
        qDebug() << "SFTPAccess: SFTP check process timedout.";

    // Cleanup
    delete sftpAvailableCheckProcess;

    return parseSFTPAvailableCheckResp(sftpAvailableCheckResp);
}

bool SFTPAccess::parseSFTPAvailableCheckResp(QString sftpResp){

    if(sftpResp.isEmpty()){
        qDebug() << "SFTPAccess: SFTP available check response is empty.";
        return false;
    }

    qDebug() << "SFTPAccess: SFTP available check response\t" << sftpResp;
    QStringList respList = sftpResp.split("\n");
    
    if(respList.at(0).contains("not running")){
        qDebug() << "SFTPAccess: SFTP is not running..";
        return false;
    }
    else if(respList.count() > 3 && respList.at(2).contains("is running")){
        qDebug() << "SFTPAccess: SFTP is running";
        return true;
    }
    else{
        qDebug() << "SFTPAccess: Invalid SFTP available check response";
        return false;
    }
}

void SFTPAccess::enableSFTP(){
    qDebug() << "SFTPAccess: Enabling SFTP access...";

    if(!sftpAvailableCheck()){
        
        // 01. Temporarily start the SFTP server using "service ssh start"
        int enableSFTPExitCode = 1; // Generic error code.

        QProcess *enableSFTPProcess = new QProcess();
        QObject::connect(enableSFTPProcess, &QProcess::finished, [&](int exitCode, QProcess::ExitStatus exitStatus){ qDebug() << "SFTPAccess: SFTP enable process finished. " << exitCode; enableSFTPExitCode = exitCode;});
        QObject::connect(enableSFTPProcess, &QProcess::errorOccurred, [=](QProcess::ProcessError error){ qDebug() << "SFTPAccess: SFTP enable process error occurred. " << error; });
        
        enableSFTPProcess->setProgram("sudo");
        enableSFTPProcess->setArguments(QStringList() << "service" << "ssh" << "start");
        enableSFTPProcess->start();

        if (!enableSFTPProcess->waitForStarted())
            // Wait for the process to start
            qDebug() << "SFTPAccess: Failed to start the SFTP enable process.";
        else if (!enableSFTPProcess->waitForFinished())
            // Wait until the process is completed.
            qDebug() << "SFTPAccess: SFTP enable process timedout.";
        else
            qDebug() << "SFTP enable exit code: " << enableSFTPExitCode;

        // Cleanup
        delete enableSFTPProcess;

        // 02. Configure SSH for temporary SFTP use.
        addSFTPConfig();
    }
}

void SFTPAccess::addSFTPConfig(){
    qDebug() << "SFTPAccess: Configure SSH for temporary SFTP use...";

    QProcess *configSFTPProcess = new QProcess();
    QObject::connect(configSFTPProcess, &QProcess::finished, [&](int exitCode, QProcess::ExitStatus exitStatus){ qDebug() << "SFTPAccess: SFTP enable process finished. " << exitCode;});
    QObject::connect(configSFTPProcess, &QProcess::errorOccurred, [=](QProcess::ProcessError error){ qDebug() << "SFTPAccess: SFTP enable process error occurred. " << error; });
    
    enableSFTPProcess->setProgram("sudo");
    enableSFTPProcess->setArguments(QStringList() << "service" << "ssh" << "start");
    enableSFTPProcess->start();

    if (!enableSFTPProcess->waitForStarted())
        // Wait for the process to start
        qDebug() << "SFTPAccess: Failed to start the SFTP enable process.";
    else if (!enableSFTPProcess->waitForFinished())
        // Wait until the process is completed.
        qDebug() << "SFTPAccess: SFTP enable process timedout.";
    else
        qDebug() << "SFTP enable exit code: " << enableSFTPExitCode;

    // Cleanup
    delete enableSFTPProcess;

}
