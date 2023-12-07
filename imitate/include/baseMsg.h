#ifndef __BASEMSG_H__
#define __BASEMSG_H__

#include <stdint.h>

using namespace std;

class BaseMsg
{
    public:
        enum TYPE {
            SERVICE = 1,
        };
        uint8_t type;
        char load[999999]; // 用于检测内存泄漏
        virtual ~BaseMsg(){}
};

#endif