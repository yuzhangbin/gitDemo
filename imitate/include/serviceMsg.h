#ifndef __SERVICEMSG_H__
#define __SERVICEMSG_H__

#include "baseMsg.h"
#include <memory>

using namespace std;

class SerciceMsg : public BaseMsg 
{
    public:
        uint32_t souce;
        shared_ptr<char> buff;
        size_t size;
};

#endif