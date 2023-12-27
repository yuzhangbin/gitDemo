#include "../include/socketWorker.h"
#include "../include/socket_epoll.h"

#include <iostream>
#include <unistd.h>

using namespace std;

void SocketWorker::init()
{
    cout << "SocketWorker Init" << endl;
    this->efd = make_shared<epoller>();
    cout << this->efd->fd() << "wo shi jian ting de epollID" << endl;
}

void SocketWorker::operator()()
{
    while (true)
    {
        cout << "SocketWorker starting" << endl;
        usleep(100);
    }
    
}

void SocketWorker::addEvent(int fd)
{
    cout << "SocketWorker::addEvent fd = " << fd <<endl;

}