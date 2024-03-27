#include "NetworkManager.h"
#include "FileReceive.h"

int main(){
    NetworkManager discovery;
    FileReceive server(discovery.getLocalHostInterface().at(0));
    return 0;
}
