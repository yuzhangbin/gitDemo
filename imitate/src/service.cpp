#include "../include/service.h"

Service::Service()
{
    spinlock_init(&queueLock);
    spinlock_init(&inGlobalLock);
}

Service::~Service()
{
    spinlock_destroy(&queueLock);
    spinlock_destroy(&inGlobalLock);
}

void Service::pushMsg(shared_ptr<BaseMsg> message)
{
    spinlock_lock(&queueLock);
    {
        msgQueue.push(message);
    }
    spinlock_unlock(&queueLock);
}

shared_ptr<BaseMsg> Service::popMsg()
{
    shared_ptr<BaseMsg> retMsg = nullptr;
    spinlock_lock(&queueLock);
    {
        if(!msgQueue.empty())
        {
            retMsg = msgQueue.front();
            msgQueue.pop();
        }
    }
    spinlock_unlock(&queueLock);
    return retMsg;
}

// 服务初始化回调
void Service::onInit()
{

}

// 服务收到消息回调
void Service::onMessage(shared_ptr<BaseMsg> message)
{
    // 测试使用
    if(message->type == BaseMsg::TYPE::SERVICE)
    {
        auto msg = dynamic_pointer_cast<ServiceMsg>(message);
        cout<<id<<"  "<<msg->buff<<endl;
        Sunnet *sunnetInstance = Sunnet::instance();
        auto msgRet = sunnetInstance->makeMsg(id, new char[ 9999999] { 'p', 'i', 'n', 'g', '\0' }, 9999999);
        sunnetInstance->send(msg->source, msgRet);
    }
    else
    {
        cout<<id<<"zan shi wei ding yi"<<endl;
    }
}

// 服务退出
void Service::onExit()
{

}

// 处理一条消息，返回值判断是否处理
bool Service::processMsg()
{
    // 这样做的目的是缩小临界区，提高效率
    shared_ptr<BaseMsg> msg = popMsg();
    if(msg)
    {
        onMessage(msg);
        return true;
    }
    else
    {
        return false;
    }
}

// 处理多条消息，返回值判断是否处理
void Service::processMsgs(int max)
{
    for(int i = 0; i < max; i++)
    {
        bool ok = processMsg();
        if(!ok)
        {
            break;
        }
    }
}

void Service::SetInGlobal(bool isIn) // 线程安全
{
    spinlock_lock(&inGlobalLock);
    {
        inGlobal = isIn;
    }
    spinlock_unlock(&inGlobalLock);
}