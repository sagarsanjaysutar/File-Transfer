#ifndef UTILS_H
#define UTILS_H

#include <QProcess>
#include <QSharedPointer>

class Utils{

private:
    Utils();

    static QSharedPointer<Utils> m_instance;
    

public:
    ~Utils();
    static void destroyInstance(Utils *obj);
    static QSharedPointer<Utils> getUtils();
    int startProcess(QString cmdStr);
};

#endif