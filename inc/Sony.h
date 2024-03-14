#include <iostream>
#include <QObject>

using namespace std;

class Sony : public QObject{
    Q_OBJECT
public:
    Sony(QObject* parent = nullptr);
    ~Sony();
};