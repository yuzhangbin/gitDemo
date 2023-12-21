#include "worker.h"
#include <iostream>
#include <unistd.h>

using namespace std;

void Worker::operator()()
{
    Sunnet *sunnetInstance = Sunnet::instance();
    cout << "working id:" << id << endl;
    while (true)
    {
        // cout << "working id:" << id << endl;
        shared_ptr<Service> svr = sunnetInstance->popGlobalQueue();
        if(!svr)
        {
            // cout<<"quan ju fu wu dui lie li mian mei you fu wu"<<endl;
            // usleep(100000);
            // 休眠线程，等到有消息的时候再唤醒
            // cout << "zusezaizhelima1" << endl;
            sunnetInstance->workerWait();
            // cout << "zusezaizhelima2" << endl;
        }
        else
        {
            svr->processMsgs(eachNum);
            // 处理完之后再检查该服务是否还有未处理的消息
            checkAndPutGlobal(svr);
        }
    }
}

void Worker::checkAndPutGlobal(shared_ptr<Service> service)
{
    if(service->isExiting)
    {
        cout<<"gai fu wu zheng zai tui chu"<<endl;
        return ;
    }
    Sunnet *sunnetInstance = Sunnet::instance();
    spinlock_lock(&service->queueLock);
    {
        if(!service->msgQueue.empty())
        {
            sunnetInstance->pushGlobalQueue(service);
        }
        else
        {
            service->SetInGlobal(false);
        }
    }
    spinlock_unlock(&service->queueLock);
}