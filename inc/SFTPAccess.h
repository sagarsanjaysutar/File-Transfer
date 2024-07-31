#ifndef SFTPACCESS_H
#define SFTPACCESS_H

#include <QProcess>

class SFTPAccess : public QObject{

Q_OBJECT

public:

    SFTPAccess();
    ~SFTPAccess();

    Q_INVOKABLE void enableSFTP();

    Q_INVOKABLE void disableSFTP();

};

#endif