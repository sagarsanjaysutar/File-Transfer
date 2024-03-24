#include <QString>
#include <QDebug>
#include <QFile>
#include <QIODevice>
#include <QRegExp>
#include <QHostAddress>

int main(){
    QStringList dataList;
    QFile file("r.txt");
    file.open(QIODevice::ReadOnly);
    
    while (!file.atEnd()) {
        QString line = file.readLine();
        dataList << line.remove("\n");       
    }
    file.close();

    for(int idx = 0; idx < dataList.count(); idx++){
        QString line = dataList.at(idx);
        if(line.contains("Nmap scan report")){
            if(dataList.at(idx + 4).contains("open")){
                // qDebug() << line << " opened.";
                QRegExp rx("[0-9]{1,3}.[0-9]{1,3}.[0-9]{1,3}.[0-9]{1,3}");
                int xx = rx.indexIn(line);
                // hosts << QHostAddress(rx.cap(0));
            }
            else{
                qDebug() << line << " closed.";
            }
        }
    }
    
    
    return 0;
}