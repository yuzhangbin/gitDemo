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
    pthread_mutex_destroy(&sleepMtx);
    pthread_cond_destroy(&sleepCond);
}

void Sunnet::start()
{
    cout<<"sunnet start"<<endl;
    pthread_rwlock_init(&servicesLock, NULL);
    spinlock_init(&globalSpinLock);
    pthread_mutex_init(&sleepMtx, NULL);
    pthread_cond_init(&sleepCond, NULL);
    this->startWorker();
    this->startSocket();
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
        /* 1 2 4 8 16 工作线程处理消息的数量呈指数级增长
           因为锁的原因，怕效率太低，所以可以一次处理很多的消息，但是这样有的线程数里的消息数量很大
           又会造成延迟，所以就使用指数级增长的方式，前面消息数小的解决延迟问题
           后面消息数大的解决效率问题
        */
        thread *tw = new thread(*worker);
        this->workers.push_back(worker);
        this->workerThreads.push_back(tw);
    }
}

void Sunnet::startSocket()
{
    socketWorker = new SocketWorker();
    socketWorker->init();
    socketWorkerThread = new thread(*socketWorker);
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
    svr->isExiting = true; // 1、避免在删除的时候其它服务向该服务发送消息
    //2、避免退出的时候，多线程产生冲突
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
    shared_ptr<Service> svr = getService(serviceID);
    if(!svr)
    {
        cout<<"mei you zhao dao gai fu wu"<<endl;
        return ;
    }
    svr->pushMsg(msg);  // 里面已经有锁了
    bool inGlobal = false;
    spinlock_lock(&svr->inGlobalLock);
    {
        if(!svr->inGlobal)
        {
            pushGlobalQueue(svr);
            svr->inGlobal = true;
            inGlobal = true;
        }
    }
    spinlock_unlock(&svr->inGlobalLock);
    if(inGlobal)
    {
        // 插入全局消息队列唤醒线程
        this->checkAndWeakUp();
    }
}

shared_ptr<BaseMsg> Sunnet::makeMsg(uint32_t source, char *buff, int len)
{
    auto msg = make_shared<ServiceMsg>();
    msg->type = BaseMsg::TYPE::SERVICE;
    msg->source = source;
    // buff是通过new出来的，不要再被其它堆对象引用，不然导致智能指针的引用计数错误
    msg->buff = shared_ptr<char>(buff);
    msg->size = len;
    return msg;
}

void Sunnet::checkAndWeakUp()
{
    if(sleepCount == 0)
    {
        return ;
    }
    if(WORKER_NUM - sleepCount <= globalLen)
    {
        cout << "wake up" << endl;
        pthread_cond_signal(&sleepCond);
    }
}

void Sunnet::workerWait()
{
    pthread_mutex_lock(&sleepMtx);
    sleepCount++;
    pthread_cond_wait(&sleepCond, &sleepMtx); //进入休眠解开上面锁住的sleepMtx，阻塞在这里，被唤醒时，对sleepMtx加锁，继续流程
    sleepCount--;
    pthread_mutex_unlock(&sleepMtx);
}