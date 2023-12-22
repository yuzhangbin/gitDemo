#include "../include/socketWorker.h"

#include <iostream>
#include <unistd.h>

using namespace std;

void SocketWorker::init()
{
    cout << "SocketWorker Init" << endl;
    epollFd = sp_create();
    cout << epollFd << "0000000000000" << endl;
    assert(epollFd > 0);
}

void SocketWorker::operator()()
{
    while (true)
    {
        cout << "SocketWorker starting" << endl;
        usleep(100);
    }
    
}