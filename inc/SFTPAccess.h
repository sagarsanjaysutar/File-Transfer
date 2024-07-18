#ifndef SFTPACCESS_H
#define SFTPACCESS_H

#include <QProcess>

class SFTPAccess{

public:

    SFTPAccess();
    ~SFTPAccess();

private:

    void enableSFTP();

    bool parseSFTPAvailableCheckResp(QString);

    bool sftpAvailableCheck();

    void addSFTPConfig();
};

#endif