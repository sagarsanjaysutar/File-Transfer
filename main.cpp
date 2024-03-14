#include <iostream>
#include <QPointer>
#include "inc/Sony.h"

using namespace std;



class Camera{
public:
    Camera() { cout << "Camera constructor called.\n"; }
    Sony m_obj;

    void randomFunction(){
        cout << "Random function.\n";

        // if(m_obj == nullptr){

        //     cout << "Sony is null\n";
        //     m_obj = new Sony();
        // }
        // else
        //     cout << "Sony is not null.\n";
    }
};

int main(){
    Camera obj;
    obj.randomFunction();
    return 0;
}