#include <iostream>
#include "../include/sunnet.h"

using namespace std;

Sunnet *Sunnet::inst = nullptr;

Sunnet::Sunnet()
{

}

Sunnet::~Sunnet()
{
    pthread_rwlock_destroy(&servicesLock);
    spinlock_destroy(&globalSpinLock);
}

void Sunnet::start()
{
    cout<<"sunnet start"<<endl;
    pthread_rwlock_init(&servicesLock, NULL);
    spinlock_init(&globalSpinLock);
    this->startWorker();
}

void Sunnet::wait()
{
    if(this->workerThreads[0])
    {
        this->workerThreads[0]->join();
    }
}

Sunnet *Sunnet::instance()
{
    if (inst == nullptr) {
        inst = new Sunnet();
    }
    return inst;
}

void Sunnet::startWorker()
{
    for(int i = 0; i < WORKER_NUM; i++)
    {
        Worker * worker = new Worker();
        worker->id = i;
        worker->eachNum = i << 2;
        thread *tw = new thread(*worker);
        this->workers.push_back(worker);
        this->workerThreads.push_back(tw);
    }
}

uint32_t Sunnet::newService(shared_ptr<string> type)
{
    auto svr = make_shared<Service>();
    svr->type = type;
    pthread_rwlock_wrlock(&servicesLock);
    {
        svr->id = maxServiceID;
        maxServiceID++;
        services.emplace(svr->id, svr);
    }
    pthread_rwlock_unlock(&servicesLock);
    svr->onInit();
    return svr->id;
}

shared_ptr<Service> Sunnet::getService(uint32_t serviceID)
{
    shared_ptr<Service> svr = nullptr;
    pthread_rwlock_rdlock(&servicesLock);
    {
        unordered_map<uint32_t, shared_ptr<Service>>::iterator iter = services.find(serviceID);
        if(iter != services.end())
        {
            svr = iter->second;
        }
    }
    pthread_rwlock_unlock(&servicesLock);
    return svr;
}

void Sunnet::killService(uint32_t serviceID)
{
    shared_ptr<Service> svr = getService(serviceID);
    if(!svr)
    {
        return ;
    }
    svr->onExit();
    svr->isExiting = true; // 避免在删除的时候其它服务向该服务发送消息
    pthread_rwlock_wrlock(&servicesLock);
    {
        services.erase(serviceID);
    }
    pthread_rwlock_unlock(&servicesLock);
}

shared_ptr<Service> Sunnet::popGlobalQueue()
{
    shared_ptr<Service> svr = nullptr;
    spinlock_lock(&globalSpinLock);
    {
        if(!globalQueue.empty())
        {
            svr = globalQueue.front();
            globalQueue.pop();
            globalLen--;
        }
    }
    spinlock_unlock(&globalSpinLock);
    return svr;
}
void Sunnet::pushGlobalQueue(shared_ptr<Service> svr)
{
    spinlock_lock(&globalSpinLock);
    {
        globalQueue.push(svr);
        globalLen++;
    }
    spinlock_unlock(&globalSpinLock);
}

// 过程分为两个步骤，其一，发送方（服务1）将消息插入接收方（服务2）的消息队列中；其二，如果接收方（服务2）不在全局队列中，将它插入全局队列，使工作线程能够处理它。
void Sunnet::send(uint32_t serviceID, shared_ptr<BaseMsg> msg)
{

}