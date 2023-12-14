#ifndef __SERVICE_H__
#define __SERVICE_H__

#include <stdlib.h>
#include <memory>
#include <queue>

#include "baseMsg.h"
#include "spinLock.h"

using namespace std;

class Service
{
    public:
        uint32_t id;
        shared_ptr<string> type;
        bool isExiting = false;
        queue<shared_ptr<BaseMsg>> msgQueue;
        struct spinlock queueLock;
    public:
        Service();
        ~Service();
        void pushMsg(shared_ptr<BaseMsg> message);
        shared_ptr<BaseMsg> popMsg();
        void onInit();
        void onMessage(shared_ptr<BaseMsg> message);
        void onExit();
        bool processMsg();
        void processMsgs(int max);
    public:
        //标记是否在全局队列，true:表示在队列中，或正在处理[避免在全局队列中遍历消耗时间]
        bool inGlobal = false;
        struct spinlock inGlobalLock;
        void SetInGlobal(bool isIn); // 线程安全
};

#endif
