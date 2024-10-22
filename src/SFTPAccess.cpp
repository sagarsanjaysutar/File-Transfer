#include "SFTPAccess.h"
#include <QDebug>
#include <QProcessEnvironment>

// Use the preprocessor macro
#define Q(x) #x
#define QUOTE(x) Q(x)
#ifdef PROJECT_SOURCE_DIR
static const QString projectDir(QUOTE(PROJECT_SOURCE_DIR));
#else
static const QString projectDir("");
#endif

SFTPAccess::SFTPAccess(QString hostIPAddress) : m_hostIPAddress(hostIPAddress)
{
    qDebug() << "SFTPAccess: Constructor called.";
}

SFTPAccess::~SFTPAccess()
{
    qDebug() << "SFTPAccess: Destructor called.";
}

void SFTPAccess::disableSFTP()
{
    qDebug() << "SFTPAccess: Disabling SFTP access...";

    // Stop the SFTP server.
    int disableSFTPExitCode = 1; // Generic error code.

    QProcess *disableSFTPProcess = new QProcess();
    QObject::connect(disableSFTPProcess, &QProcess::finished, [&](int exitCode, QProcess::ExitStatus exitStatus)
                     { disableSFTPExitCode = exitCode; });
    QObject::connect(disableSFTPProcess, &QProcess::readyReadStandardOutput, [&]()
                     { qDebug() << "SFTPAccess: Disable SFTP output: " << disableSFTPProcess->readAllStandardOutput(); });
    QObject::connect(disableSFTPProcess, &QProcess::readyReadStandardError, [&]()
                     {
        qDebug() << "SFTPAccess: Disable SFTP error output:" << disableSFTPProcess->readAllStandardError();; });
    QObject::connect(disableSFTPProcess, &QProcess::errorOccurred, [=](QProcess::ProcessError error)
                     { qDebug() << "SFTPAccess: SFTP disable process error occurred. " << error; });

    disableSFTPProcess->setProgram(projectDir + "/scripts/enableSFTP.sh");
    disableSFTPProcess->setArguments(QStringList() << "stop");
    disableSFTPProcess->start();

    if (!disableSFTPProcess->waitForStarted())
        // Wait for the process to start
        qDebug() << "SFTPAccess: Failed to start the SFTP disable process.";
    else if (!disableSFTPProcess->waitForFinished())
        // Wait until the process is completed.
        qDebug() << "SFTPAccess: SFTP disable process timedout.";
    else
        qDebug() << "SFTPAccess: SFTP disable exit code: " << disableSFTPExitCode;

    // Cleanup
    delete disableSFTPProcess;
}

void SFTPAccess::enableSFTP()
{
    qDebug() << "SFTPAccess: Enabling SFTP access...";

    // Temporarily start the SFTP server.
    int enableSFTPExitCode = 1; // Generic error code.

    QProcess *enableSFTPProcess = new QProcess();
    QObject::connect(enableSFTPProcess, &QProcess::finished, [&](int exitCode, QProcess::ExitStatus exitStatus)
                     { enableSFTPExitCode = exitCode; });
    QObject::connect(enableSFTPProcess, &QProcess::readyReadStandardOutput, [&]()
                     { qDebug() << "SFTPAccess: Enable SFTP output: " << enableSFTPProcess->readAllStandardOutput(); });
    QObject::connect(enableSFTPProcess, &QProcess::readyReadStandardError, [&]()
                     { qDebug() << "SFTPAccess: Enable SFTP error output:" << enableSFTPProcess->readAllStandardError(); });
    QObject::connect(enableSFTPProcess, &QProcess::errorOccurred, [=](QProcess::ProcessError error)
                     { qDebug() << "SFTPAccess: SFTP enable process error occurred. " << error; });

    enableSFTPProcess->setProgram(projectDir + "/scripts/enableSFTP.sh");
    enableSFTPProcess->start();

    if (!enableSFTPProcess->waitForStarted())
        // Wait for the process to start
        qDebug() << "SFTPAccess: Failed to start the SFTP enable process.";
    else if (!enableSFTPProcess->waitForFinished())
        // Wait until the process is completed.
        qDebug() << "SFTPAccess: SFTP enable process timedout.";
    else
        qDebug() << "SFTPAccess: SFTP enable exit code: " << enableSFTPExitCode;

    // Cleanup
    delete enableSFTPProcess;
}