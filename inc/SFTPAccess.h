#ifndef SFTPACCESS_H
#define SFTPACCESS_H

#include <QProcess>
#include <libssh/libssh.h>
#include <stdlib.h>

class SFTPAccess : public QObject
{

    Q_OBJECT

public:
    SFTPAccess(QString hostIPAddress);
    ~SFTPAccess();

    Q_INVOKABLE void enableSFTP();

    Q_INVOKABLE void disableSFTP();

private:
    QString m_hostIPAddress;
};

#endif