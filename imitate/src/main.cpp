#include <iostream>
#include "sunnet.h"

void test(Sunnet *sunnetInstance)
{
    shared_ptr<string> type = make_shared<string>("ping");
    uint32_t ping1 = sunnetInstance->newService(type);
    uint32_t ping2 = sunnetInstance->newService(type);
    uint32_t ping3 = sunnetInstance->newService(type);
}

int main()
{
    Sunnet *sunnetInstance = Sunnet::instance();
    sunnetInstance->start();
    test(sunnetInstance);
    sunnetInstance->wait();
    return 0;
}