#include <iostream>
#include "../include/sunnet.h"

using namespace std;

Sunnet *Sunnet::inst = nullptr;

Sunnet::Sunnet()
{

}

void Sunnet::start()
{
    cout<<"sunnet start"<<endl;
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