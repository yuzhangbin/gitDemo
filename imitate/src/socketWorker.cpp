#include "../include/socketWorker.h"

#include <iostream>
#include <unistd.h>

using namespace std;

void SocketWorker::init()
{
    cout << "SocketWorker Init" << endl;
}

void SocketWorker::operator()()
{
    while (true)
    {
        cout << "SocketWorker starting" << endl;
        usleep(100);
    }
    
}